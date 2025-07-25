#include <chrono>  // NOLINT [build/c++11]
#include <map>
#include <string>

#include "DataCollectionManager.h"
#include "OBJParser.h"
#include "Package.h"
#include "PriorityShipping.h"
#include "SimulationModel.h"
#include "WebServer.h"

//--------------------  Controller ----------------------------
bool stopped = false;
/// A Transit Service that communicates with a web page through web sockets.  It
/// also acts as the controller in the model view controller pattern.
class TransitService : public JsonSession, public IController {
 public:
  TransitService()
      : model(*this), start(std::chrono::system_clock::now()), time(0.0) {
    // Drones are now created in SimulationModel constructor
  }

  /// Handles specific commands from the web server
  void receiveCommand(const std::string &cmd, const JsonObject &data,
                      JsonObject &returnValue) {
    try {
      // std::cout << cmd << ": " << data << std::endl;
      if (cmd == "CreateEntity") {
        model.createEntity(data);
      } else if (cmd == "SetGraph") {
        std::string path = data["filePath"];
        model.setGraph(routing::OBJGraphParser(path));
      } else if (cmd == "ScheduleTrip") {
        std::string priority = "Standard";  // Default priority
        if (data.contains("priority")) {
          priority = std::string(data["priority"]);
        }
        model.scheduleTrip(data, priority);
      } else if (cmd == "ChangePriority") {
        std::cout << "Command received to changePriority" << std::endl;
        std::string packageName = data["packageName"];
        std::string priority = data["priority"];
        std::cout << "Attempting to change priority for package: "
                  << packageName << " to " << priority << std::endl;
        bool success = model.changePackagePriority(packageName, priority);
        std::cout << data << std::endl;

        if (!success) {
          std::cout << "Priority change failed." << std::endl;
          // std::cout << "Priority change failed." << std::endl;
          notify("Could not change priority for " + packageName +
                 ". Package may already be picked up already or doesnt exist.");
        } else {
          std::cout << "Priority change finally working lol." << std::endl;
          // std::cout << "Priority change finally working lol." << std::endl;

          notify("Successfully changed the priority status for " + packageName +
                 " to " + priority);
        }
      } else if (cmd == "GetDeliveryQueue") {
        // Send the current delivery queue information to the client
        JsonObject queueInfo = model.getDeliveryQueueInfo();
        sendEventToView("DeliveryQueueInfo", queueInfo);
      }

      else if (cmd == "ping") {
        if (data.contains("message"))
          std::cout << std::string(data["message"]) << std::endl;
        returnValue["response"] = data;
      } else if (cmd == "Update") {
        updateEntites.clear();
        std::chrono::time_point<std::chrono::system_clock> end =
            std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end - start;
        double delta = diff.count() - time;
        time += delta;
        double simSpeed = data["simSpeed"];
        delta *= simSpeed;
        if (delta > 0.1) {
          for (float f = 0.0; f < delta; f += 0.01) {
            model.update(0.01);
          }
        } else {
          model.update(delta);
        }
        for (auto &[id, entity] : updateEntites) {
          sendEntity("UpdateEntity", *entity);
        }
      } else if (cmd == "stopSimulation") {
        std::cout << "Stop command administered\n";
        stopped = true;
        model.stop();
      } else if (cmd == "writeStats") {
        // handle data collection here
        DataCollectionManager::getInstance()
            ->exportLog();  // should take care of everything from here
      }
    } catch (const std::exception &e) {
      std::cerr << "Error handling command " << cmd << ": " << e.what()
                << std::endl;
      // Create an error response
      JsonObject errorDetails;
      errorDetails["command"] = cmd;
      errorDetails["error"] = std::string(e.what());
      sendEventToView("CommandError", errorDetails);
    }
  }

  void sendEntity(const std::string &event, const IEntity &entity,
                  bool includeDetails = true) {
    // JsonObject details = entity.GetDetails();
    JsonObject details;
    if (includeDetails) {
      details["details"] = entity.getDetails();
    }
    details["id"] = entity.getId();
    Vector3 pos_ = entity.getPosition();
    Vector3 dir_ = entity.getDirection();
    JsonArray pos = {pos_.x, pos_.y, pos_.z};
    JsonArray dir = {dir_.x, dir_.y, dir_.z};
    details["pos"] = pos;
    details["dir"] = dir;
    std::string col_ = entity.getColor();
    if (col_ != "") details["color"] = col_;

    // For packages, include priority info if available
    if (const Package *package = dynamic_cast<const Package *>(&entity)) {
      if (!package->getPriorityName().empty()) {
        details["priority"] = package->getPriorityName();
        details["priorityLevel"] = package->getPriorityLevel();
      }
    }

    sendEventToView(event, details);
  }

  void addEntity(const IEntity &entity) {
    sendEntity("AddEntity", entity, true);
  }

  void updateEntity(const IEntity &entity) {
    updateEntites[entity.getId()] = &entity;
  }

  void removeEntity(const IEntity &entity) {
    JsonObject details;
    details["id"] = entity.getId();
    updateEntites.erase(entity.getId());
    sendEventToView("RemoveEntity", details);
  }

  /// Allows messages to be passed back to the view
  void sendEventToView(const std::string &event, const JsonObject &details) {
    JsonObject eventData;
    eventData["event"] = event;
    eventData["details"] = details;
    sendMessage(eventData.toString());
  }

  // Helper to send notifications
  void notify(const std::string &message) {
    JsonObject details;
    details["message"] = message;
    sendEventToView("Notification", details);
  }

 private:
  // Simulation Model
  SimulationModel model;
  // Used for tracking time since last update
  std::chrono::time_point<std::chrono::system_clock> start;
  // The total time the server has been running.
  double time;
  // Current entities to update
  std::map<int, const IEntity *> updateEntites;
};

/// The main program that handles starting the web sockets service.
int main(int argc, char **argv) {
  if (argc > 1) {
    int port = std::atoi(argv[1]);
    std::string webDir = std::string(argv[2]);
    WebServer<TransitService> server(port, webDir);
    while (!stopped) {
      server.service();
    }
  } else {
    std::cout
        << "Usage: ./build/bin/transit_service <port> apps/transit_service/web/"
        << std::endl;
  }

  return 0;
}
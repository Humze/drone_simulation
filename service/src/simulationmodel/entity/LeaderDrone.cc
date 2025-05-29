#define _USE_MATH_DEFINES
#include "LeaderDrone.h"

#include <cmath>
#include <iostream>  //used to move drone to random location after charging

#include "AstarStrategy.h"
#include "BeelineStrategy.h"
#include "BfsStrategy.h"
#include "DataCollectionManager.h"
#include "DfsStrategy.h"
#include "DijkstraStrategy.h"
#include "Package.h"
#include "SimulationModel.h"

LeaderDrone::LeaderDrone(const JsonObject &obj) : Drone(obj) {
  available = true;
}

LeaderDrone::~LeaderDrone() {
  if (toPackage) delete toPackage;
  if (toFinalDestination) delete toFinalDestination;
  if (toChargingStation) delete toChargingStation;
}

void LeaderDrone::getNextDelivery() {
  if (model && model->scheduledDeliveries.size() > 0) {
    Drone::setPackage(model->scheduledDeliveries.front());
    model->scheduledDeliveries.pop_front();

    Package *package = getPackage();

    if (package) {
      std::string message = getName() + " heading to: " + package->getName();
      notifyObservers(message);
      available = false;
      pickedUp = false;

      Vector3 packagePosition = package->getPosition();
      Vector3 finalDestination = package->getDestination();

      toPackage = new BeelineStrategy(position, packagePosition);

      std::string strat = package->getStrategyName();
      if (strat == "astar") {
        toFinalDestination = new AstarStrategy(
            packagePosition, finalDestination, model->getGraph());
      } else if (strat == "dfs") {
        toFinalDestination = new DfsStrategy(packagePosition, finalDestination,
                                             model->getGraph());
      } else if (strat == "bfs") {
        toFinalDestination = new BfsStrategy(packagePosition, finalDestination,
                                             model->getGraph());
      } else if (strat == "dijkstra") {
        toFinalDestination = new DijkstraStrategy(
            packagePosition, finalDestination, model->getGraph());
      } else {
        toFinalDestination =
            new BeelineStrategy(packagePosition, finalDestination);
      }
    }
  }
}

double LeaderDrone::getBatteryHealth() { return battery_health; }
void LeaderDrone::travelToCharger() {
  Vector3 dronePosition = this->getPosition();
  toChargingStation =
      new BeelineStrategy(dronePosition, charging_station_location);
}

void LeaderDrone::depleteBattery(double dt) {
  battery_health = battery_health - .01 * (dt);
  if (battery_health < 0) {
    battery_health = 0;
  }
}
void LeaderDrone::chargeBattery(double dt) {
  while (battery_health < max_battery_health) {
    battery_health = battery_health + 0.01 * (dt);
  }

  if (battery_health > max_battery_health) {
    battery_health = max_battery_health;
  }
}

void LeaderDrone::update(double dt) {  // setup pointer to current package
  Package *package = Drone::getPackage();
  // DCM integration
  DataCollectionManager *dcm = DataCollectionManager::getInstance();
  dcm->logEvent(this, "timesteps_of_entity", 1.0);
  // Calculate how far it moved since last frame
  double diff = this->lastPosition.dist(this->position);

  // Update the position for next time
  this->lastPosition = this->position;

  // Update distance traveled
  this->distanceTraveled += diff;

  dcm->logEvent(this, "distance_traveled", this->distanceTraveled);

  depleteBattery(dt);
  // std::cout <<"DEBUG:"<<getName()<< " battery_health:
  // "<<battery_health<<std::endl;

  if (available && (battery_health > critical_battery_health)) {
    getNextDelivery();
  }

  if (available && (battery_health <= critical_battery_health) && !pickedUp) {
    available = false;
    this->travelToCharger();
  }

  if (toPackage &&
      (battery_health > critical_battery_health)) {  // going to package
    toPackage->move(this, dt);

    if (toPackage->isCompleted()) {
      std::string message = getName() + " picked up: " + package->getName();
      package->isPickedUp();
      notifyObservers(message);
      delete toPackage;
      toPackage = nullptr;
      pickedUp = true;
    }
  } else if (toFinalDestination && (battery_health > critical_battery_health) &&
             pickedUp) {
    toFinalDestination->move(this, dt);

    if (package && pickedUp) {
      // deplete faster when carrying a package
      depleteBattery(dt);
      package->setPosition(position);
      package->setDirection(direction);
    }

    if (toFinalDestination->isCompleted()) {
      std::string message = getName() + " dropped off: " + package->getName();

      notifyObservers(message);

      dcm->logEvent(this, "packages_delivered", 1.0);

      delete toFinalDestination;
      toFinalDestination = nullptr;
      package->handOff();
      package->DeliveredPackage();
      Drone::setPackage(nullptr);
      available = true;
      pickedUp = false;
    }
  } else if (toChargingStation) {
    toChargingStation->move(this, dt);
    if (package && pickedUp) {
      package->setPosition(position);
      package->setDirection(direction);
    }

    if (toChargingStation->isCompleted()) {
      delete toChargingStation;
      toChargingStation = nullptr;
      chargeBattery(dt);
      if (!pickedUp) {
        available = true;
      }
    }
  }
  // claude.ai was used to validate the if statements for behavior when battery
  // is below critical value
  /* prompts used: does this capture the proper states of battery?
                  else if (battery_health <= critical_battery_health){
                  if (package && pickedUp){
                  if (battery_health > emergency_battery_health){
                  if (helper_drone != nullptr){
                  }else{}
                  }else{}
                  }else if (toPackage && !pickedUp)
                      {if (package){
                        }
                      }
                      pickedUp = false;
                      this->travelToCharger();
                    }
                  }
  additional prompts: does it capture the right combinations
  */
  else if (battery_health <= critical_battery_health) {
    // when the battery is equal to or below critical, set availability to
    // false, so it doesn't go pick up a package
    available = false;
    std::cout << getName() << "DEBUG: Battery CRITICAL\n";

    // if carrying a package
    if (package && pickedUp) {
      if (battery_health > emergency_battery_health) {
        std::string message = getName() + " requesting handoff";
        notifyHelperDroneObservers(message);
        // DCM Integration: Log how many times requests assistence
        dcm->logEvent(this, "handoff_requests", 1.0);

        HelperDrone *helper_drone = nullptr;
        helper_drone = selectClosestHelperDrone();
        if (helper_drone != nullptr) {
          std::string helper_name = helper_drone->getName();
          std::string leader_name = this->getName();

          std::string message2 =
              helper_name + " accepted handoff for " + leader_name;
          dcm->logEvent(helper_drone, "handoff_accepts", 1.0);

          helper_drone->notify(message2);

          // Handoff the package safely
          helper_drone->getNextDelivery(this->getPackage());

          Drone::setPackage(nullptr);
        } else {
          // package not handed off and leader not abandoning package
          std::string message3 =
              getName() +
              "requesting handoff again, no helper drones available";
          notifyHelperDroneObservers(message3);
        }
      } else {
        std::cout << getName() << "Battery EMERGENCY" << std::endl;
        std::string message4 =
            getName() +
            "going to charging station with package, no helpers available.";
        model->notify(message4);
      }
    } else if (toPackage && !pickedUp) {
      std::string message5 =
          getName() + " abandoning pickup due to critical battery level.";
      model->notify(message5);

      // Return the package to scheduled deliveries
      if (package) {
        model->scheduledDeliveries.push_back(package);
        model->sortScheduledDeliveries();
        Drone::setPackage(nullptr);
      }

      // Clear the toPackage path
      delete toPackage;
      toPackage = nullptr;
    }
    pickedUp = false;

    this->travelToCharger();
  }
}

// claude.ai used for help with working with map
// prompts used: how to store values with map std::map<HelperDrone *, double>
// handoffResponses
void LeaderDrone::receiveHelperResponse(HelperDrone *drone, double distance) {
  if (drone != nullptr) {
    handoffResponses[drone] = distance;
  }
}

HelperDrone *LeaderDrone::selectClosestHelperDrone() {
  // Debug output
  // std::cout << "DEBUG: Entering selectClosestHelperDrone()" << std::endl;
  // std::cout << "DEBUG: handoffResponses size: " << handoffResponses.size() <<
  // std::endl;

  // Check if there are any responses
  if (handoffResponses.empty()) {
    std::cout << "DEBUG: No handoff responses received" << std::endl;
    return nullptr;
  }

  HelperDrone *closestDrone = nullptr;
  double smallest_distance = -1.0;

  // Iterate through the map, claude.ai helped with efficient iteration and
  // choice of structure prompts used were: when to use map vs set vs other
  // structure, show me how to iterate with for loop through
  // std::map<HelperDrone *, double> handoffResponses;
  for (const auto &pair : handoffResponses) {
    HelperDrone *drone = pair.first;
    double distance = pair.second;

    // Make sure the drone pointer is valid
    if (drone == nullptr) {
      std::cerr << "WARNING: Null drone in handoffResponses" << std::endl;
      continue;
    }

    // Get the smallest distance and set the closest drone
    if (smallest_distance < 0 || distance < smallest_distance) {
      smallest_distance = distance;
      closestDrone = drone;
    }
  }

  // Debug
  // std::cout << "DEBUG: Selected drone: " << (closestDrone ?
  // closestDrone->getName() : "nullptr") << std::endl;

  return closestDrone;
}
Package *LeaderDrone::getPackage() { return Drone::getPackage(); };

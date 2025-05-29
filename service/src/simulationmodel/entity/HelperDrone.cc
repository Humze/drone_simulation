#define _USE_MATH_DEFINES
#include "HelperDrone.h"

#include <cmath>
#include <limits>

#include "AstarStrategy.h"
#include "BeelineStrategy.h"
#include "BfsStrategy.h"
#include "DataCollectionManager.h"
#include "DfsStrategy.h"
#include "DijkstraStrategy.h"
#include "Drone.h"
#include "LeaderDrone.h"
#include "Package.h"
#include "SimulationModel.h"

HelperDrone::HelperDrone(const JsonObject& obj) : Drone(obj) {
  available = true;
}
// adding somecomment to help test
HelperDrone::~HelperDrone() {
  if (toPackage) delete toPackage;
  if (toFinalDestination) delete toFinalDestination;
}

void HelperDrone::getNextDelivery(Package* leader_package) {
  Drone::setPackage(leader_package);

  Package* package = getPackage();

  if (package != nullptr) {
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

void HelperDrone::update(double dt) {
  // DCM integration
  DataCollectionManager* dcm = DataCollectionManager::getInstance();
  dcm->logEvent(this, "timesteps_of_entity", 1.0);
  // Calculate how far it moved since last frame
  double diff = this->lastPosition.dist(this->position);

  // Update the position for next time
  this->lastPosition = this->position;

  // Update distance traveled
  this->distanceTraveled += diff;

  dcm->logEvent(this, "distance_traveled", this->distanceTraveled);

  // if (available) {
  //      getNextDelivery();
  // } we wait so this code no longer needed
  Package* package = getPackage();
  if (toPackage) {
    toPackage->move(this, dt);

    if (toPackage->isCompleted()) {
      std::string message = getName() + " picked up: " + package->getName();
      notifyObservers(message);
      delete toPackage;
      toPackage = nullptr;
      pickedUp = true;
    }
  } else if (toFinalDestination) {
    toFinalDestination->move(this, dt);

    if (package && pickedUp) {
      package->setPosition(position);
      package->setDirection(direction);
    }

    if (toFinalDestination->isCompleted()) {
      std::string message = getName() + " dropped off: " + package->getName();

      dcm->logEvent(this, "packages_delivered", 1.0);

      notifyObservers(message);
      delete toFinalDestination;
      toFinalDestination = nullptr;
      package->handOff();
      Drone::setPackage(nullptr);
      available = true;
      pickedUp = false;
    }
  }
}

void HelperDrone::notify(const std::string& message) const {
  model->notify(message);
}
void HelperDrone::notifyWithPublisher(const std::string& message,
                                      IPublisher* publisher) const {
  if (!publisher) {
    // Just perform regular notification if publisher is null
    model->notify(message);
    return;
  }
  // used claude ai to learn how to check typeid for objects in c++
  // prompt used: how to check typeof object in c++
  // prompt used: using dynamic cast vs typeid
  // Instead of dynamic_cast, use typeid for safety
  if (typeid(*publisher) == typeid(LeaderDrone)) {
    // Safe cast since we verified the type
    LeaderDrone* leader = static_cast<LeaderDrone*>(publisher);

    if (message.find("requesting handoff") != std::string::npos) {
      Vector3 leader_position = leader->getPosition();

      Vector3 helper_position = getPosition();

      double distance = leader_position.dist(helper_position);

      if (available) {
        // Remove const_cast if possible by making this method non-const
        leader->receiveHelperResponse(const_cast<HelperDrone*>(this), distance);
      }
    }
  }
}

Package* HelperDrone::getPackage() { return Drone::getPackage(); };

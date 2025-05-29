#include "SimulationModel.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

#include "ATC.h"
#include "AirplaneFactory.h"
#include "DataCollectionManager.h"
#include "DroneFactory.h"
#include "ExpeditedShipping.h"
#include "HelicopterFactory.h"
#include "HelperDrone.h"
#include "HumanFactory.h"
#include "LeaderDrone.h"
#include "NoRushShipping.h"
#include "PackageFactory.h"
#include "PriorityShipping.h"
#include "RobotFactory.h"
#include "StandardShipping.h"

SimulationModel::SimulationModel(IController &controller)
    : controller(controller) {
  entityFactory.addFactory(new DroneFactory());
  entityFactory.addFactory(new PackageFactory());
  entityFactory.addFactory(new RobotFactory());
  entityFactory.addFactory(new HumanFactory());
  entityFactory.addFactory(new HelicopterFactory());
  entityFactory.addFactory(new AirplaneFactory());
  ATC::getInstance().addObserver(this);

  DataCollectionManager *dcm_instance = DataCollectionManager::getInstance();
  dcm_instance->addObserver(this);
}

SimulationModel::~SimulationModel() {
  // Delete dynamically allocated variables
  for (auto &[id, entity] : entities) {
    DataCollectionManager *dcm_instance = DataCollectionManager::getInstance();
    dcm_instance->removeEntity(entity);

    delete entity;
  }
  delete graph;
}

IEntity *SimulationModel::createEntity(const JsonObject &entity) {
  std::string name = entity["name"];
  JsonArray position = entity["position"];
  std::cout << name << ": " << position << std::endl;

  IEntity *myNewEntity = nullptr;
  if (myNewEntity = entityFactory.createEntity(entity)) {
    myNewEntity->linkModel(this);
    controller.addEntity(*myNewEntity);
    entities[myNewEntity->getId()] = myNewEntity;
    myNewEntity->addObserver(this);

    // dcm integration

    if (dynamic_cast<Drone *>(myNewEntity) ||
        dynamic_cast<Helicopter *>(myNewEntity) ||
        dynamic_cast<Airplane *>(myNewEntity)) {
      std::cout << "Adding entity to ATC" << std::endl;
      ATC::getInstance().addEntity(myNewEntity);
    }

    // For helper drones, connect them to leader drones
    if (entity.contains("type")) {
      std::string drone_type = entity["type"];
      if (drone_type.compare("helper_drone") == 0) {
        // Unwrap the helper drone from all the decorators decorator
        // std::cout << "myNewEntity" << typeid(*myNewEntity).name() <<
        // std::endl;
        if (DroneATCDecorator *atcDecorator =
                dynamic_cast<DroneATCDecorator *>(myNewEntity)) {
          // std::cout << "atcDecorator" << typeid(atcDecorator).name() <<
          // std::endl;
          DroneColorDecorator *droneColorDrone =
              dynamic_cast<DroneColorDecorator *>(
                  atcDecorator->getDecoratedEntity());
          // std::cout << "droneColorDrone" << typeid(*droneColorDrone).name()
          // << std::endl;

          HelperDrone *helperDrone = dynamic_cast<HelperDrone *>(
              droneColorDrone->getDecoratedEntity());
          // std::cout << "helperDrone" << typeid(*helperDrone).name() <<
          // std::endl;

          if (helperDrone) {
            // Connect to all leader drones
            for (auto const &[id, entityPtr] : entities) {
              if (entityPtr != myNewEntity && entityPtr) {
                // peel the ATC and Color decorator for the leader drone
                if (DroneATCDecorator *leaderATCDecorator =
                        dynamic_cast<DroneATCDecorator *>(entityPtr)) {
                  // First peel off ATC decorator
                  DroneColorDecorator *leaderColorDecorator =
                      dynamic_cast<DroneColorDecorator *>(
                          leaderATCDecorator->getDecoratedEntity());
                  if (leaderColorDecorator) {
                    // peel off color decorator to get the base leader drone
                    LeaderDrone *leaderDrone = dynamic_cast<LeaderDrone *>(
                        leaderColorDecorator->getDecoratedEntity());
                    if (leaderDrone) {
                      leaderDrone->addHelperDroneObserver(helperDrone);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  if (myNewEntity) {
    DataCollectionManager *dcm_instance = DataCollectionManager::getInstance();
    dcm_instance->createLog(myNewEntity);
    std::cout << "Created log for entity pointer: " << myNewEntity->getName()
              << std::endl;
  }
  return myNewEntity;
}

void SimulationModel::removeEntity(int id) { removed.insert(id); }

/// INTERNAL TESTING SCHEDULETRIP METHOD,,, Schedules a Delivery for an object
/// in the scene
/*
void SimulationModel::scheduleTrip(const JsonObject &details)
{
  std::string name = details["name"];
  JsonArray start = details["start"];
  JsonArray end = details["end"];
  std::cout << name << ": " << start << " --> " << end << std::endl;

  Robot *receiver = nullptr;

  for (auto &[id, entity] : entities)
  {
    if (name == entity->getName())
    {
      if (Robot *r = dynamic_cast<Robot *>(entity))
      {
        if (r->requestedDelivery)
        {
          receiver = r;
          break;
        }
      }
    }
  }

  Package *package = nullptr;

  for (auto &[id, entity] : entities)
  {
    if (name + "_package" == entity->getName())
    {
      if (Package *p = dynamic_cast<Package *>(entity))
      {
        if (p->requiresDelivery())
        {
          package = p;
          break;
        }
      }
    }
  }

  if (receiver && package)
  {
    if (!package->getPriority())
    {
      srand(time(0));
      int randomNum = rand() % 3;
      switch (randomNum)
      {
      case 0:
        std::cout << "Standard Shipping" << std::endl;
        package->setPriority(new StandardShipping());
        break;
      case 1:
        std::cout << "NoRush Shipping" << std::endl;
        package->setPriority(new NoRushShipping());
        break;
      case 2:
        std::cout << "Expedited Shipping" << std::endl;
        package->setPriority(new ExpeditedShipping());
        break;
      }
    }
    package->initDelivery(receiver);

    std::string strategyName = details["search"];
    package->setStrategyName(strategyName);
    scheduledDeliveries.push_back(package);
    std::cout << scheduledDeliveries.size() << std::endl;
    sortScheduledDeliveries();
    std::cout << "This is the packages priority levels: " <<
package->getPriorityLevel() << std::endl;

    controller.sendEventToView("DeliveryScheduled", details);
  }
} */

void SimulationModel::scheduleTrip(const JsonObject &details,
                                   const std::string &priority) {
  std::string name = details["name"];
  JsonArray start = details["start"];
  JsonArray end = details["end"];
  std::cout << name << ": " << start << " --> " << end << " [" << priority
            << "]" << std::endl;

  Robot *receiver = nullptr;
  for (auto &[id, entity] : entities) {
    if (name == entity->getName()) {
      if (Robot *r = dynamic_cast<Robot *>(entity)) {
        if (r->requestedDelivery) {
          receiver = r;
          break;
        }
      }
    }
  }

  Package *package = nullptr;
  for (auto &[id, entity] : entities) {
    if (name + "_package" == entity->getName()) {
      if (Package *p = dynamic_cast<Package *>(entity)) {
        if (p->requiresDelivery()) {
          package = p;
          break;
        }
      }
    }
  }

  if (receiver && package) {
    if (!package->getPriority()) {
      if (priority == "Standard") {
        package->setPriority(new StandardShipping());
      } else if (priority == "NoRush") {
        package->setPriority(new NoRushShipping());
      } else if (priority == "Expedited") {
        package->setPriority(new ExpeditedShipping());
      } else {
        std::cout << "Unknown priority type. Defaulting to Standard."
                  << std::endl;
        package->setPriority(new StandardShipping());
      }
    }

    package->initDelivery(receiver);
    package->setScheduled(true);
    getDeliveryQueueInfo();
    std::string strategyName = details["search"];
    package->setStrategyName(strategyName);
    scheduledDeliveries.push_back(package);
    sortScheduledDeliveries();
    controller.sendEventToView("DeliveryScheduled", details);
  }
}

void SimulationModel::sortScheduledDeliveries() {
  std::sort(scheduledDeliveries.begin(), scheduledDeliveries.end(),
            [](Package *a, Package *b) {
              return a->getPriorityLevel() < b->getPriorityLevel();
            });
}

const routing::Graph *SimulationModel::getGraph() const { return graph; }

void SimulationModel::setGraph(const routing::Graph *graph) {
  if (this->graph) delete this->graph;
  this->graph = graph;
}

/// Updates the simulation
void SimulationModel::update(double dt) {
  for (auto &[id, entity] : entities) {
    entity->update(dt);
    controller.updateEntity(*entity);
  }

  for (int id : removed) {
    removeFromSim(id);
  }
  removed.clear();
  ATC::getInstance().update(dt);
}

void SimulationModel::stop(void) {}

void SimulationModel::removeFromSim(int id) {
  IEntity *entity = entities[id];
  if (entity) {
    for (auto i = scheduledDeliveries.begin(); i != scheduledDeliveries.end();
         ++i) {
      if (*i == entity) {
        scheduledDeliveries.erase(i);
        break;
      }
    }
    // DCM
    DataCollectionManager *dcm_instance = DataCollectionManager::getInstance();
    dcm_instance->removeEntity(entity);

    controller.removeEntity(*entity);
    entities.erase(id);
    delete entity;
  }
}

bool SimulationModel::changePackagePriority(const std::string &packageName,
                                            const std::string &priority) {
  for (auto &[id, entity] : entities) {
    if (entity->getName() == packageName) {
      if (Package *package = dynamic_cast<Package *>(entity)) {
        if (!package->isScheduled()) {
          std::cout << "Too late to change priority of package" << std::endl;
          return false;
        }

        if (priority == "Standard" && !package->getPackagePickedUp()) {
          std::cout << "package is set to standard" << std::endl;
          package->setPriority(new StandardShipping());
          std::cout << package->getName() << " new priority is "
                    << package->getPriorityName() << std::endl;
        }

        else if (priority == "NoRush" && !package->getPackagePickedUp()) {
          std::cout << "package is set to norush" << std::endl;

          package->setPriority(new NoRushShipping());
          std::cout << package->getName() << " new priority is "
                    << package->getPriorityName() << std::endl;
        }

        else if (priority == "Expedited" && !package->getPackagePickedUp()) {
          std::cout << "package is set to expedited" << std::endl;

          package->setPriority(new ExpeditedShipping());
          std::cout << package->getName() << " new priority is "
                    << package->getPriorityName() << std::endl;
        } else {
          return false;
        }
        sortScheduledDeliveries();
        return true;
      }
    }
  }
  return false;
}

JsonObject SimulationModel::getDeliveryQueueInfo() const {
  JsonObject queueInfo;
  JsonArray queueArray;

  for (const Package *pkg : scheduledDeliveries) {
    JsonObject pkgInfo;
    pkgInfo["name"] = pkg->getName();
    pkgInfo["priority"] = pkg->getPriorityName();
    pkgInfo["priorityLevel"] = pkg->getPriorityLevel();
    pkgInfo["isPickedUp"] = pkg->getPackagePickedUp();
    pkgInfo["isDelivered"] = pkg->getDeliveredPackage();
    Vector3 pos = pkg->getPosition();
    JsonArray posArray = {pos.x, pos.y, pos.z};
    pkgInfo["position"] = posArray;

    queueArray.push(pkgInfo);
  }

  queueInfo["packages"] = queueArray;
  return queueInfo;
}

void SimulationModel::notify(const std::string &message) const {
  JsonObject details;
  details["message"] = message;
  this->controller.sendEventToView("Notification", details);
}

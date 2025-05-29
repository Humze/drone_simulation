/**
 * AI Acknowledgement: Anthropic Claude 3.7 Sonnet
 * - Used for assistence with Doxygen Commenting
 * - How to do file I/O operations for writing to csv file in C++
 * - Writing decriptive debugging messages
 * - How to iterate through a map in C++
 * - How to get current time to add to exported csv filename
 */
#include "DataCollectionManager.h"

#include <ctime>
#include <fstream>
#include <iostream>

DataCollectionManager* DataCollectionManager::instance = nullptr;

DataCollectionManager* DataCollectionManager::getInstance() {
  if (instance == nullptr) {
    instance = new DataCollectionManager();
  }
  return instance;
}

void DataCollectionManager::createLog(IEntity* entity) {
  if (entity != nullptr) {
    if (logMap.count(entity->getId()) == 0) {
      // Entity id must be added to log
      logMap.emplace(entity->getId(), std::map<std::string, double>{});

      idToName.emplace(entity->getId(), entity->getName());

      // std::cout << "Log has been initialized for entity: " <<
      // entity->getName() << " (id: " << entity->getId() << ")" << std::endl;
    } else {
      // std::cout << "Log already exists for entity: " << entity->getName()<< "
      // (id: " << entity->getId() << ")" << std::endl;
    }
  } else {
    // std::cout << "ERROR: Attempted to create log for null entity" <<
    // std::endl;
  }
}

void DataCollectionManager::exportLog() {
  // seconds since 1/1/1970, from Claude
  std::time_t now = std::time(nullptr);

  std::string filename = "EntityLog_" + std::to_string(now) + ".csv";
  std::fstream fileout(filename, std::ios::out);

  fileout << "EntityName, EntityID, MetricName, MetricValue" << std::endl;
  for (const auto& entity : logMap) {
    std::string name = idToName.at(entity.first);
    std::string pointer = std::to_string(entity.first);

    for (const auto& event : entity.second) {
      fileout << name << ", " << pointer << ", " << event.first << ", "
              << event.second << std::endl;
      std::cout << "Log metric exported for " << name << ": " << event.first
                << std::endl;
    }
  }

  std::cout << "All logs exported to " << filename << std::endl;
  fileout.close();

  std::string message = "Logs have been successfully exported to " + filename;
  this->notifyObservers(message);
}

void DataCollectionManager::logEvent(IEntity* entity, std::string eventName,
                                     double metric) {
  // increment eventName by metric

  if (logMap.count(entity->getId())) {
    auto& logOfEntity = logMap[entity->getId()];
    if (logOfEntity.count(eventName)) {
      logOfEntity[eventName] += metric;  // increment
    } else {
      // event is being updated for the first time
      logOfEntity[eventName] = metric;
    }
  } else {
    // std::cout << "ERROR: Shouldn't get here, Log for entity doesn't exist: "
    // << entity->getName() << " (id: " << entity->getId() << ")" << std::endl;
  }
}

void DataCollectionManager::removeEntity(IEntity* entity) {
  if (logMap.count(entity->getId()) > 0) {
    logMap.erase(entity->getId());
    // std::cout << "Entity removed from log: " << entity->getName() <<
    // std::endl;
  }
}

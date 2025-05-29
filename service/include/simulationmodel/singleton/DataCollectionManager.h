/**
 * AI Acknowledgement: Anthropic Claude 3.7 Sonnet
 * - Used for assistence with Doxygen Commenting
 */

#ifndef IDATACOLLECTIONMANAGER_H_
#define IDATACOLLECTIONMANAGER_H_

#include <string>

#include "IDataLogger.h"
#include "IEntity.h"
#include "IPublisher.h"

/**
 * @class DataCollectionManager
 * @brief Singleton class implementing IDataLogger for logging and IPublisher
 * for notifications
 *
 * The DataCollectionManager provides centralized tracking and logging of entity
 * metrics throughout the simulation. It stores event data by entity ID and can
 * export this data to CSV files for analysis.
 */

class DataCollectionManager : public IDataLogger, public IPublisher {
 public:
  /**
   * @brief Gets the singleton instance of DataCollectionManager
   * @return Pointer to the single DataCollectionManager instance
   *
   * If the instance doesn't exist yet, it creates one before returning
   */
  static DataCollectionManager* getInstance();

  /**
   * @brief Exports the simulation log to a CSV file
   *
   * Creates a file named "EntityLog.csv" containing all logged metrics
   * for each entity, then notifies observers of successful export
   */
  void exportLog() override;

  /**
   * @brief Initializes logging for a specific entity
   * @param entity Pointer to the entity to track
   *
   * Creates necessary data structures for logging events related to the
   * specified entity. If the entity already has logging initialized,
   * this function has no effect.
   */
  void createLog(IEntity* entity) override;

  /**
   * @brief Records a metric event for an entity
   * @param entity Pointer to the entity generating the event
   * @param eventName Name of the metric being recorded
   * @param metric Value of the metric to add
   *
   * If the metric already exists for this entity, the value is incremented.
   * Otherwise, a new metric entry is created with the specified value.
   */
  void logEvent(IEntity* entity, std::string eventName, double metric) override;

  /**
   * @brief Removes an entity from the logging system
   * @param entity Pointer to the entity to remove
   *
   * Deletes all logged data associated with the specified entity
   */
  void removeEntity(IEntity* entity) override;

 private:
  /**
   * @brief Private constructor prevents direct instantiation
   *
   * Part of the singleton pattern implementation
   */
  DataCollectionManager() {}

  static DataCollectionManager* instance;
};

#endif  // IDATACOLLECTIONMANAGER_H_
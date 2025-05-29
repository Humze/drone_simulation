/**
 * AI Acknowledgement: Anthropic Claude 3.7 Sonnet
 * - Used for assistence with Doxygen Commenting
 */
#ifndef IDATALOGGER_H_
#define IDATALOGGER_H_

#include <string>

#include "IEntity.h"

/**
 * @class IDataLogger
 * @brief Interface defining logging operations for simulation entities
 *
 * Provides methods for creating, updating, and exporting logs of entity metrics
 * during simulation execution. Classes implementing this interface can track
 * and record various metrics associated with entities.
 */
class IDataLogger {
 public:
  /**
   * @brief Exports the simulation log to a file
   *
   * Implementing classes should provide a mechanism to export log data
   * in a format suitable for analysis.
   */
  virtual void exportLog() = 0;

  /**
   * @brief Initializes logging for a specific entity
   * @param entity Pointer to the entity to track
   *
   * Creates necessary data structures for logging events related to the
   * specified entity.
   */
  virtual void createLog(IEntity* entity) = 0;

  /**
   * @brief Records a metric event for an entity
   * @param entity Pointer to the entity generating the event
   * @param eventName Name of the metric being recorded
   * @param metric Value of the metric to add
   *
   * Creates or updates a metric entry for the specified entity.
   */
  virtual void logEvent(IEntity* entity, std::string eventName,
                        double metric) = 0;

  /**
   * @brief Removes an entity from the logging system
   * @param entity Pointer to the entity to remove
   *
   * Deletes all logged data associated with the specified entity.
   */
  virtual void removeEntity(IEntity* entity) = 0;

 protected:
  std::map<int, std::map<std::string, double>>
      logMap;  ///< Maps entity IDs to their metrics
  std::map<int, std::string>
      idToName;  ///< Maps entity IDs to names for readable logs
};

#endif  // IDATALOGGER_H_
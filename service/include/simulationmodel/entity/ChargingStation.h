#ifndef CHARGINGSTATION_H
#define CHARGINGSTATION_H
#include "IEntity.h"

/**
 * @class ChargingStation
 * @brief Represents a CHARGINGSTATION in a physical system
 *
 * CHARGINGSTATION is an entity placed on the map that drones are hard set to
 * return to.
 */
class ChargingStation : public IEntity {
 public:
  /**
   * @brief Constructor
   * @param obj JSON object containing the CHARGINGSTATION's information
   */
  ChargingStation(const JsonObject& obj);

  /**
   * @brief Updates the station stae
   *
   * @param dt difference in time since last update
   */
  void update(double dt);

 private:
};

#endif  // CHARGINGSTATION_H

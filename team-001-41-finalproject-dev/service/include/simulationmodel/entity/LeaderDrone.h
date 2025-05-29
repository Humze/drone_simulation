#ifndef LEADERDRONE_H_
#define LEADERDRONE_H_
#include <string>
#include <vector>

#include "Drone.h"
#include "HelperDrone.h"
#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"

class Package;

// Represents a drone in a physical system.
// Drones move using euler integration based on a specified
// velocity and direction.
/**
 * @class LeaderDrone
 * @brief Represents a drone in a physical system. Drones move using euler
 * integration based on a specified velocity and direction.
 */
class LeaderDrone : public Drone {
 public:
  /**
   * @brief Drones are created with a name
   * @param obj JSON object containing the drone's information
   */
  LeaderDrone(const JsonObject &obj);

  /**
   * @brief Destructor
   */
  ~LeaderDrone();

  /**
   * @brief Returns package it has assigned
   * @return type Package*, returns pointer to current package
   */
  Package *getPackage();

  /**
   * @brief Gets the next delivery in the scheduler
   */
  void getNextDelivery();

  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   */
  void update(double dt);
  /**
   * @brief Depletes the drone's battery
   * @param dt double change in time, to delete the battery in proportion of
   * change in time
   */
  void depleteBattery(double dt);
  /**
   * @brief Updates the drone's position
   * @return type double with current battery health
   */
  double getBatteryHealth();
  /**
   * @brief Sets the strategy type from current position to charging station.
   */
  void travelToCharger();
  /**
   * @brief Fills up the battery attribute to mimic charging at the charging
   * station
   * @param dt type double, so that it fills up in relation to type passed
   */
  void chargeBattery(double dt);

  const std::set<const IObserver *> &getHelperObservers() const;
  /**
   * @brief Removing the copy constructor operator
   * so that drones cannot be copied
   */
  LeaderDrone(const LeaderDrone &drone) = delete;

  /**
   * @brief Removing the assignment operator
   * so that drones cannot be copied.
   */
  LeaderDrone &operator=(const LeaderDrone &drone) = delete;
  /**
   * @brief Receives the response from helper drones
   * @param drone type HelperDrone*, pointer to the helper drone responding
   * @param distance type double with the distance of the helper drone from
   * leader drone
   */
  void receiveHelperResponse(HelperDrone *drone, double distance);
  /**
   * @brief selects the closest helper drone for assistance to leader drone
   * @return type HelperDrone, since it returns a pointer to the helper drone
   * that will assist
   */
  HelperDrone *selectClosestHelperDrone();

  bool available = false;
  bool pickedUp = false;

 private:
  IStrategy *toPackage = nullptr;
  IStrategy *toFinalDestination = nullptr;
  IStrategy *toChargingStation = nullptr;
  double battery_health = 100.0;
  double max_battery_health = 100.0;
  double critical_battery_health = 25.0;
  double emergency_battery_health = 13.0;
  std::map<HelperDrone *, double> handoffResponses;

  Vector3 charging_station_location = Vector3(92, 254, -124);
  std::string drone_type = "LeaderDrone";

  // dcm distance logging
  double distanceTraveled = 0;
  Vector3 lastPosition;
};

#endif

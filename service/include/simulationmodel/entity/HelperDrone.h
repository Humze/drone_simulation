#ifndef HELPERDRONE_H_
#define HELPERDRONE_H_
#include <string>
#include <vector>

#include "Drone.h"
#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"

class Package;

/**
 * @class HelperDrone
 * @brief Represents a helper drone in a physical system. Drones move using
 * euler integration based on a specified velocity and direction. These drones
 * assist leader drones through the observer pattern, as observers.
 */
class HelperDrone : public Drone, public IObserver {
 public:
  /**
   * @brief Drones are created with a name
   * @param obj JSON object containing the drone's information
   */
  HelperDrone(const JsonObject& obj);

  /**
   * @brief Destructor
   */
  ~HelperDrone();

  /**
   * @brief Gets the next delivery in the scheduler
   */
  void getNextDelivery(Package* leader_package);

  /**
   * @brief Returns package it has assigned
   * @return type Package*, returns a pointer to drone's current package
   */
  Package* getPackage();

  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   */
  void update(double dt);

  /**
   * @brief Removing the copy constructor operator
   * so that drones cannot be copied.
   */
  HelperDrone(const Drone& drone) = delete;

  /**
   * @brief Removing the assignment operator
   * so that drones cannot be copied.
   */
  HelperDrone& operator=(const Drone& drone) = delete;

  /**
   * @brief notification helper method that includes the leader publisher.
   * @param message type string describing the update
   * @param publisher type IPublisher* pointer to leader drone that message is
   * sent to.
   */
  void notifyWithPublisher(const std::string& message,
                           IPublisher* publisher) const;
  /**
   * @brief notification method for publishers.
   * @param message string describing the message to notify with
   */
  void notify(const std::string& message) const;

  bool available = false;
  bool pickedUp = false;

 private:
  IStrategy* toPackage = nullptr;
  IStrategy* toFinalDestination = nullptr;
  std::string drone_type = "HelperDrone";

  // dcm distance logging
  double distanceTraveled = 0;
  Vector3 lastPosition;
};

#endif

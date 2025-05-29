#ifndef PACKAGE_DECORATOR_H_
#define PACKAGE_DECORATOR_H_

#include "Drone.h"
#include "FlyingEntityDecorator.h"
#include "HelperDrone.h"
#include "LeaderDrone.h"
#include "Package.h"
#include "Robot.h"

/**
 * @brief DroneDecorator is a decorator that decorates a Drone
 */
class DroneDecorator : public FlyingEntityDecorator<Drone> {
 public:
  /**
   * @brief Constructor for DroneDecorator
   * @param d The Drone to decorate
   */
  DroneDecorator(Drone* d) : FlyingEntityDecorator<Drone>(d) {}

  /**
   * @brief Get the next delivery
   */
  virtual void getNextDelivery() { return sub->getNextDelivery(); };

  /**
   * @brief Get the package
   * @return The package
   */
  virtual Package* getPackage() { return sub->getPackage(); };

  /**
   * @brief Get the next delivery
   * @param p The package to get the next delivery for
   */
  virtual void getNextDelivery(Package* p) {
    HelperDrone* helper = static_cast<HelperDrone*>(sub);
    helper->getNextDelivery(p);
  }

  /**
   * @brief Check if the drone is a LeaderDrone
   * @return True if the drone is a LeaderDrone, false otherwise
   */
  bool isLeaderDrone() const;

  /**
   * @brief Check if the drone is a HelperDrone
   * @return True if the drone is a HelperDrone, false otherwise
   */
  bool isHelperDrone() const;
};

#endif
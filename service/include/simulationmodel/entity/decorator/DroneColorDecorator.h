#ifndef DRONE_COLOR_DECORATOR_H
#define DRONE_COLOR_DECORATOR_H

#include "Drone.h"
#include "DroneDecorator.h"
#include "HelperDrone.h"
#include "LeaderDrone.h"
#include "Package.h"

/**
 * @brief DroneColorDecorator is a decorator that decorates a Drone with a color
 */
class DroneColorDecorator : public DroneDecorator {
 private:
  double hue = 0;
  double saturation = 0;
  double light = 0;
  double distToPackage = -1;
  double distToDestination = -1;

 public:
  /**
   * @brief Constructor for DroneColorDecorator
   * @param d The Drone to decorate)
   */
  DroneColorDecorator(Drone* d, double = 0, double = 0, double = 0);
  /**
   * @brief Get the color of the Drone
   * @return The color of the Drone
   */
  std::string getColor() const;
  /**
   * @brief Set the color of the Drone
   */
  void setColor(double h, double s, double l);
  /**
   * @brief Update the color of the Drone
   */
  void update(double dt);
  /**
   * @brief Get the decorated entity
   * @return The decorated entity
   */
  Drone* getDecoratedEntity() const;
};

#endif  // DRONE_COLOR_DECORATOR_H
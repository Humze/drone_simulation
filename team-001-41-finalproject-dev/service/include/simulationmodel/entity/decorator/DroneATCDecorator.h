#ifndef DRONE_ATC_DECORATOR_H_
#define DRONE_ATC_DECORATOR_H_

#include "Drone.h"
#include "DroneDecorator.h"

/**
 * @brief DroneATCDecorator is a decorator that reroutes the drone to avoid
 * collision
 */
class DroneATCDecorator : public DroneDecorator {
 public:
  /**
   * @brief Constructor for DroneATCDecorator
   * @param drone The drone to decorate
   */
  DroneATCDecorator(Drone* drone);

  /**
   * @brief Reroute the drone to avoid collision
   */
  void reroute() override;

  /**
   * @brief Update the drone's position and direction
   */
  void update(double dt) override;
  Drone* getDecoratedEntity() const;

 private:
};

#endif

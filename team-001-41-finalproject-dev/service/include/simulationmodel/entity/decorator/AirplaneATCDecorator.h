#ifndef AIRPLANE_ATC_DECORATOR_H_
#define AIRPLANE_ATC_DECORATOR_H_

#include "AirplaneDecorator.h"

/**
 * @brief AirplaneATCDecorator is a decorator that reroutes the airplane to
 * avoid collision
 */
class AirplaneATCDecorator : public AirplaneDecorator {
 public:
  /**
   * @brief Constructor for AirplaneATCDecorator
   * @param airplane The airplane to decorate
   */
  AirplaneATCDecorator(Airplane* airplane);

  /**
   * @brief Reroute the airplane to avoid collision
   */
  void reroute();

  /**
   * @brief Update the airplane's position and direction
   */
  void update(double dt);

 private:
};

#endif

#ifndef HELICOPTER_ATC_DECORATOR_H_
#define HELICOPTER_ATC_DECORATOR_H_

#include "HelicopterDecorator.h"

/**
 * @brief HelicopterATCDecorator is a decorator that reroutes the helicopter to
 * avoid collision
 */
class HelicopterATCDecorator : public HelicopterDecorator {
 public:
  /**
   * @brief Constructor for HelicopterATCDecorator
   * @param helicopter The helicopter to decorate
   */
  HelicopterATCDecorator(Helicopter* helicopter);

  /**
   * @brief Reroute the helicopter to avoid collision
   */
  void reroute();

  /**
   * @brief Update the helicopter's position and direction
   */
  void update(double dt);

 private:
};

#endif

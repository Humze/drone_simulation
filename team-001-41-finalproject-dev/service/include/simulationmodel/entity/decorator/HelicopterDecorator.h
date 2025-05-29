#ifndef HELICOPTER_DECORATOR_H_
#define HELICOPTER_DECORATOR_H_

#include "FlyingEntityDecorator.h"
#include "Helicopter.h"

/**
 * @brief HelicopterDecorator is a decorator that decorates a Helicopter
 */
class HelicopterDecorator : public FlyingEntityDecorator<Helicopter> {
 public:
  /**
   * @brief Constructor for HelicopterDecorator
   * @param h The Helicopter to decorate
   */
  HelicopterDecorator(Helicopter* h) : FlyingEntityDecorator<Helicopter>(h) {}
};

#endif

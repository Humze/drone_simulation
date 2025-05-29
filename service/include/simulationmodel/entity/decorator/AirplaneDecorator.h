#ifndef AIRPLANE_DECORATOR_H_
#define AIRPLANE_DECORATOR_H_

#include "Airplane.h"
#include "FlyingEntityDecorator.h"

/**
 * @brief AirplaneDecorator is a decorator that decorates an Airplane
 */
class AirplaneDecorator : public FlyingEntityDecorator<Airplane> {
 public:
  /**
   * @brief Constructor for AirplaneDecorator
   * @param a The Airplane to decorate
   */
  AirplaneDecorator(Airplane* a) : FlyingEntityDecorator<Airplane>(a) {}
};

#endif

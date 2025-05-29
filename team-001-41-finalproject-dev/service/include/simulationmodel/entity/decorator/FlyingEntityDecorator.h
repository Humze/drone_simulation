#ifndef FLYING_ENTITY_DECORATOR_H_
#define FLYING_ENTITY_DECORATOR_H_

#include "BeelineStrategy.h"
#include "IEntity.h"
#include "IEntityDecorator.h"
#include "IStrategy.h"

/**
 * @brief FlyingEntityDecorator is a decorator that decorates a FlyingEntity
 */
template <std::derived_from<IEntity> T = IEntity>
class FlyingEntityDecorator : public IEntityDecorator<T> {
 public:
  /**
   * @brief Constructor for FlyingEntityDecorator
   * @param entity The entity to decorate
   */
  FlyingEntityDecorator(T* entity)
      : IEntityDecorator<T>(entity), reroutedDestination(nullptr) {}

  /**
   * @brief Destructor for FlyingEntityDecorator
   */
  virtual ~FlyingEntityDecorator() {
    if (reroutedDestination) {
      delete reroutedDestination;
    }
  }

  /**
   * @brief Check if the entity is rerouted
   * @return True if the entity is rerouted, false otherwise
   */
  virtual bool isRerouted() { return rerouted; }

  /**
   * @brief Reroute the entity
   */
  virtual void reroute() {}

  /**
   * @brief Update the entity
   */
  virtual void update(double dt) {}

 protected:
  IStrategy* reroutedDestination;
  bool rerouted = false;
  double timeSinceReroute = 500;
};

#endif

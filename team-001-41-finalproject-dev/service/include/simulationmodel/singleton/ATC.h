#ifndef ATC_H_
#define ATC_H_

#include <vector>

#include "AirplaneATCDecorator.h"
#include "DroneATCDecorator.h"
#include "FlyingEntityDecorator.h"
#include "HelicopterATCDecorator.h"
#include "IEntity.h"
/**
 * @class ATC
 * @brief Implements singleton pattern and keep track of all fying objects,
 * avoiding collisions
 */
class ATC : public IPublisher {
 public:
  /**
   * @brief Get the instance of the ATC
   * @return The instance of the ATC
   */
  static ATC& getInstance();

  /**
   * @brief Add an entity to the ATC
   */
  void addEntity(IEntity* entity);

  /**
   * @brief Update the ATC
   */
  void update(double dt);

 private:
  /**
   * @brief Constructor
   */
  ATC();
  /**
   * @brief Destructor
   */
  ~ATC();
  /**
   * @brief Delete the copy constructor
   */
  ATC(const ATC&) = delete;
  /**
   * @brief Delete the assignment operator
   */
  ATC& operator=(const ATC&) = delete;

  /**
   * @brief Check if two entities will collide
   */
  bool willCollide(IEntity* a, IEntity* b);

  /**
   * @brief Choose an entity to reroute
   */
  IEntity* chooseEntity(int i, int j);

  static ATC instance;
  std::vector<IEntity*> flyingEntities;
};

#endif

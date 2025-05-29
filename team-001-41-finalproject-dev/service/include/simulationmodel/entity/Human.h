#ifndef HUMAN_H_
#define HUMAN_H_

#include "IEntity.h"
#include "IStrategy.h"

/**
 * @class Human
 * @brief Represents a human in a physical system. Humans move using euler
 * integration based on a specified velocity and direction.
 */
class Human : public IEntity {
 public:
  /**
   * @brief Humans are created with a name.
   * @param obj JSON object containing the drone's information
   */
  Human(const JsonObject& obj);
  /**
   * @brief Destructor
   */
  ~Human();

  /**
   * @brief update function to update human's position.
   * @param dt type double for change in time.
   */
  void update(double dt);

 private:
  static Vector3 kellerPosition;
  IStrategy* movement = nullptr;
  bool atKeller = false;

  double distanceTraveled = 0;
  Vector3 lastPosition;
};

#endif

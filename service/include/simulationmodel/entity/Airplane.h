#ifndef AIRPLANE_H_
#define AIRPLANE_H_
#include <vector>

#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"

// Represents an Airplane in a physical system.
// Airplanes move using euler integration based on a specified
// velocity and direction.
/**
 * @class Airplane
 * @brief Represents an airplane in a physical system. Airplanes move using
 * euler integration based on a specified velocity and direction.
 */
class Airplane : public IEntity {
 public:
  /**
   * @brief Airplanes are created with a name
   * @param obj JSON object containing the airplane's information
   */
  Airplane(const JsonObject& obj);

  /**
   * @brief Destructor
   */
  ~Airplane();

  /**
   * @brief Updates the airplane's position
   * @param dt Delta time
   */
  void update(double dt);

  bool arrived = false;

 private:
  IStrategy* toDestination = nullptr;

  // dcm distance logging
  double distanceTraveled = 0;
  Vector3 lastPosition;
};

#endif

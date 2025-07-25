#define _USE_MATH_DEFINES
#include "Helicopter.h"

#include <cmath>
#include <limits>

#include "BeelineStrategy.h"
#include "DataCollectionManager.h"

Helicopter::Helicopter(const JsonObject& obj) : IEntity(obj) {
  this->lastPosition = this->position;
}

Helicopter::~Helicopter() {
  if (movement) delete movement;
}

void Helicopter::update(double dt) {
  // DCM integration
  DataCollectionManager* dcm = DataCollectionManager::getInstance();
  dcm->logEvent(this, "timesteps_of_entity", 1);

  if (movement && !movement->isCompleted()) {
    movement->move(this, dt);

    // Calculate how far it moved since last frame
    double diff = this->lastPosition.dist(this->position);

    // Update the position for next time
    this->lastPosition = this->position;

    // Update distance traveled
    this->distanceTraveled += diff;
    dcm->logEvent(this, "distance_traveled", this->distanceTraveled);
    // If traveled a mile
    if (this->distanceTraveled > 1625.0) {
      // Format a message and send to observers
      std::string message = this->getName() + " has traveled " +
                            std::to_string(++mileCounter) + " miles";
      this->notifyObservers(message);

      // DCM integration
      dcm->logEvent(this, "miles_traveled", 1.0);
      // Reset distance traveled this mile
      this->distanceTraveled = 0;
    }
  } else {
    if (movement) delete movement;
    Vector3 dest;
    dest.x = ((static_cast<double>(rand())) / RAND_MAX) * (2900) - 1400;
    dest.y = position.y;
    dest.z = ((static_cast<double>(rand())) / RAND_MAX) * (1600) - 800;
    movement = new BeelineStrategy(position, dest);
  }
}

#define _USE_MATH_DEFINES
#include "Airplane.h"

#include <cmath>
#include <limits>

#include "ATC.h"
#include "BeelineStrategy.h"
#include "DataCollectionManager.h"
#include "Package.h"
#include "SimulationModel.h"

Airplane::Airplane(const JsonObject& obj) : IEntity(obj) {
  this->lastPosition = this->position;
}

Airplane::~Airplane() {
  if (toDestination) delete toDestination;
}

void Airplane::update(double dt) {
  // DCM integration
  DataCollectionManager* dcm = DataCollectionManager::getInstance();
  dcm->logEvent(this, "timesteps_of_entity", 1.0);

  if (toDestination) {
    toDestination->move(this, dt);

    // Calculate how far it moved since last frame
    double diff = this->lastPosition.dist(this->position);

    // Update the position for next time
    this->lastPosition = this->position;

    // Update distance traveled
    this->distanceTraveled += diff;

    dcm->logEvent(this, "distance_traveled", this->distanceTraveled);

    if (toDestination->isCompleted()) {
      std::string message = getName() + " arrived at: destination";
      dcm->logEvent(this, "reached_dest",
                    1.0);  // counter for when airplane reaches dest

      delete toDestination;
      toDestination = nullptr;

      Vector3 newDestination;
      if (position.z > 0) {
        position.x = ((static_cast<double>(rand())) / RAND_MAX) * (2900) - 1400;
        position.y = 700;
        position.z = -800;
        newDestination.x =
            ((static_cast<double>(rand())) / RAND_MAX) * (2900) - 1400;
        newDestination.y = 700;
        newDestination.z = 800;
      } else {
        position.x = ((static_cast<double>(rand())) / RAND_MAX) * (2900) - 1400;
        position.y = 700;
        position.z = 800;
        newDestination.x =
            ((static_cast<double>(rand())) / RAND_MAX) * (2900) - 1400;
        newDestination.y = 700;
        newDestination.z = -800;
      }

      toDestination = new BeelineStrategy(position, newDestination);
    }
  } else {
    Vector3 newDestination;
    if (position.z > 0) {
      newDestination = Vector3(-1600, 700, 800);
    } else {
      newDestination = Vector3(1600, 700, -800);
    }
    toDestination = new BeelineStrategy(position, newDestination);
  }
}

#define _USE_MATH_DEFINES
#include "Human.h"

#include <cmath>
#include <limits>

#include "AstarStrategy.h"
#include "DataCollectionManager.h"
#include "SimulationModel.h"

Vector3 Human::kellerPosition(64.0, 254.0, -210.0);

Human::Human(const JsonObject& obj) : IEntity(obj) {}

Human::~Human() {
  if (movement) delete movement;
}

void Human::update(double dt) {
  // DCM integration
  DataCollectionManager* dcm = DataCollectionManager::getInstance();
  dcm->logEvent(this, "timesteps_of_entity", 1.0);

  if (movement && !movement->isCompleted()) {
    movement->move(this, dt);

    // Calculate how far it moved since last frame
    double diff = this->lastPosition.dist(this->position);

    // Update the position for next time
    this->lastPosition = this->position;

    // Update distance traveled
    this->distanceTraveled += diff;

    dcm->logEvent(this, "distance_traveled", this->distanceTraveled);

    bool nearKeller = this->position.dist(Human::kellerPosition) < 85;
    if (nearKeller && !this->atKeller) {
      std::string message = this->getName() + " visited Keller hall";
      notifyObservers(message);

      dcm->logEvent(this, "visited_keller", 1.0);
    }
    atKeller = nearKeller;
  } else {
    if (movement) delete movement;
    Vector3 dest;
    dest.x = ((static_cast<double>(rand())) / RAND_MAX) * (2900) - 1400;
    dest.y = position.y;
    dest.z = ((static_cast<double>(rand())) / RAND_MAX) * (1600) - 800;
    if (model) movement = new AstarStrategy(position, dest, model->getGraph());
  }
}

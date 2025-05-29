#include "DroneATCDecorator.h"

#include "Drone.h"
#include "DroneColorDecorator.h"
#include "Package.h"

DroneATCDecorator::DroneATCDecorator(Drone* drone) : DroneDecorator(drone) {}

void DroneATCDecorator::reroute() {
  Vector3 position = sub->getPosition();
  Vector3 direction = sub->getDirection();

  double angle = 90 * M_PI / 180.0;
  Vector3 deflectDir(direction.x * cos(angle) - direction.z * sin(angle), 0.0,
                     direction.x * sin(angle) + direction.z * cos(angle));
  Vector3 newTarget = position + deflectDir * 40.0;

  if (reroutedDestination) {
    delete reroutedDestination;
  }
  reroutedDestination = new BeelineStrategy(position, newTarget);
  rerouted = true;
}

void DroneATCDecorator::update(double dt) {
  if (reroutedDestination) {
    // Try to access the package by checking for DroneColorDecorator
    DroneColorDecorator* colorDecorator =
        dynamic_cast<DroneColorDecorator*>(sub);
    Package* p = nullptr;

    Drone* baseDrone = colorDecorator->getDecoratedEntity();
    p = baseDrone->getPackage();

    // Move the drone
    reroutedDestination->move(sub, dt);

    // Update package position if it exists
    if (p && Drone::pickedUp) {
      p = baseDrone->getPackage();
      p->setPosition(sub->getPosition());
      p->setDirection(sub->getDirection());
    }

    // Check if reroute is completed
    if (reroutedDestination->isCompleted()) {
      if (timeSinceReroute > 5) {
        std::string message = sub->getName() + " finished reroute";
        sub->notifyObservers(message);
        timeSinceReroute = 0;
      }
      delete reroutedDestination;
      reroutedDestination = nullptr;
      rerouted = false;
    }
  } else {
    timeSinceReroute += dt;
    sub->update(dt);
  }
}
Drone* DroneATCDecorator::getDecoratedEntity() const { return sub; }
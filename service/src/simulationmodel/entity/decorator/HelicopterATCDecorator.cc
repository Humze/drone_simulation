#include "HelicopterATCDecorator.h"

HelicopterATCDecorator::HelicopterATCDecorator(Helicopter* helicopter)
    : HelicopterDecorator(helicopter) {}

void HelicopterATCDecorator::reroute() {
  Vector3 position = sub->getPosition();
  Vector3 direction = sub->getDirection();

  double angle = 20.0 * M_PI / 180.0;  // Convert 20 degrees to radians
  Vector3 deflectDir(direction.x * cos(angle) - direction.z * sin(angle),
                     direction.y,
                     direction.x * sin(angle) + direction.z * cos(angle));
  Vector3 newTarget = position + deflectDir * 100.0;

  if (reroutedDestination) {
    delete reroutedDestination;
  }
  reroutedDestination = new BeelineStrategy(position, newTarget);
  rerouted = true;
}

void HelicopterATCDecorator::update(double dt) {
  if (reroutedDestination) {
    reroutedDestination->move(sub, dt);
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

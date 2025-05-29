#include "AirplaneATCDecorator.h"

#include "BeelineStrategy.h"

AirplaneATCDecorator::AirplaneATCDecorator(Airplane* airplane)
    : AirplaneDecorator(airplane) {}

void AirplaneATCDecorator::reroute() {
  Vector3 position = sub->getPosition();
  Vector3 direction = sub->getDirection();

  double angle = 15.0 * M_PI / 180.0;
  Vector3 deflectDir(direction.x * cos(angle) - direction.z * sin(angle),
                     direction.y,
                     direction.x * sin(angle) + direction.z * cos(angle));
  Vector3 newTarget = position + deflectDir * 400.0;

  if (reroutedDestination) {
    delete reroutedDestination;
  }
  reroutedDestination = new BeelineStrategy(position, newTarget);
  rerouted = true;
}

void AirplaneATCDecorator::update(double dt) {
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

#include "Package.h"

#include "Robot.h"

Package::Package(const JsonObject& obj) : IEntity(obj) {}

Vector3 Package::getDestination() const { return destination; }

std::string Package::getStrategyName() const { return strategyName; }

Robot* Package::getOwner() const { return owner; }

bool Package::requiresDelivery() const { return requiresDelivery_; }

void Package::setStrategyName(std::string strategyName_) {
  strategyName = strategyName_;
}

void Package::update(double dt) {}

void Package::setPriority(PriorityShipping* newPriority) {
  if (!pickedUp && !isDelivered) {  
    if (shippingPriority) {
      delete shippingPriority;
    }
    shippingPriority = newPriority;
  }
}

PriorityShipping* Package::getPriority() const {
  return shippingPriority;
}

std::string Package::getPriorityName() const {
  if (shippingPriority) return shippingPriority->getPriorityName();
  return "Stanard";
}

int Package::getPriorityLevel() const {
  if (shippingPriority) return shippingPriority->getPriorityLevel();
  return 1;  
}

void Package::initDelivery(Robot* owner) {
  this->owner = owner;
  owner->requestedDelivery = false;
  requiresDelivery_ = false;
  destination = owner->getPosition();
  pickedUp = true;
}

void Package::handOff() {
  if (owner) {
    owner->receive(this);
  }
}

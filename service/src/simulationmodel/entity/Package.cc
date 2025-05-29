#include "Package.h"

#include "DataCollectionManager.h"
#include "Robot.h"

Package::Package(const JsonObject &obj) : IEntity(obj) {}

Vector3 Package::getDestination() const { return destination; }

std::string Package::getStrategyName() const { return strategyName; }

Robot *Package::getOwner() const { return owner; }

bool Package::requiresDelivery() const { return requiresDelivery_; }

void Package::setStrategyName(std::string strategyName_) {
  strategyName = strategyName_;
}

void Package::update(double dt) {
  // DCM integration
  DataCollectionManager *dcm = DataCollectionManager::getInstance();
  dcm->logEvent(this, "timesteps_of_entity", 1.0);
}

void Package::setPriority(PriorityShipping *newPriority) {
  if (!pickedUp && !isDelivered) {
    if (priority) {
      delete priority;
    }
    priority = newPriority;
  }
}

PriorityShipping *Package::getPriority() const { return priority; }

std::string Package::getPriorityName() const {
  if (priority) return priority->getPriorityName();
  return "Standard";
}

int Package::getPriorityLevel() const {
  if (priority) return priority->getPriorityLevel();
  return 1;
}

void Package::isPickedUp() { pickedUp = true; }

bool Package::getPackagePickedUp() const { return pickedUp; }
void Package::DeliveredPackage() { isDelivered = true; }

bool Package::getDeliveredPackage() const { return isDelivered; }

void Package::initDelivery(Robot *owner) {
  this->owner = owner;
  owner->requestedDelivery = false;
  requiresDelivery_ = false;
  destination = owner->getPosition();
}

void Package::handOff() {
  if (owner) {
    owner->receive(this);
  }
}

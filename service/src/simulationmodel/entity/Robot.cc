#include "Robot.h"

#include "DataCollectionManager.h"

Robot::Robot(const JsonObject& obj) : IEntity(obj) {}

void Robot::update(double dt) {
  // DCM integration
  DataCollectionManager* dcm = DataCollectionManager::getInstance();
  dcm->logEvent(this, "timesteps_of_entity", 1.0);
}

void Robot::receive(Package* p) { package = p; }

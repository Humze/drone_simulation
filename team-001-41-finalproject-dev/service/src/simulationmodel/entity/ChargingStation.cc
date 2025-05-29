#include "ChargingStation.h"

#include "DataCollectionManager.h"

ChargingStation::ChargingStation(const JsonObject& obj) : IEntity(obj) {}

void ChargingStation::update(double dt) {
  // DCM integration
  DataCollectionManager* dcm = DataCollectionManager::getInstance();
  dcm->logEvent(this, "timesteps_of_entity", 1.0);
}

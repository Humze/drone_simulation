#include "DroneFactory.h"

#include "ChargingStation.h"
#include "HelperDrone.h"
#include "LeaderDrone.h"

IEntity* DroneFactory::createEntity(const JsonObject& entity) {
  std::string type = entity["type"];

  if (type.compare("leader_drone") == 0) {
    return new DroneATCDecorator(
        new DroneColorDecorator(new LeaderDrone(entity), 0, 0, 100));
  }

  if (type.compare("helper_drone") == 0 || type.compare("drone") == 0) {
    return new DroneATCDecorator(
        new DroneColorDecorator(new HelperDrone(entity), 0, 0, 100));
  }

  if (type.compare("charging_station") == 0) {
    return new ChargingStation(entity);
  }

  return nullptr;
}

#include "AirplaneFactory.h"

IEntity* AirplaneFactory::createEntity(const JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("airplane") == 0) {
    std::cout << "Airplane Created" << std::endl;
    return new AirplaneATCDecorator(new Airplane(entity));
  }
  return nullptr;
}

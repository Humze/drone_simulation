#ifndef DRONE_COLOR_DECORATOR_H
#define DRONE_COLOR_DECORATOR_H

#include "Drone.h"
#include "DroneDecorator.h"
#include "Package.h"

class DroneColorDecorator : public DroneDecorator {
 private:
  double hue = 0;
  double saturation = 0;
  double light = 0;
  double distToPackage = -1;
  double distToDestination = -1;

 public:
  DroneColorDecorator(Drone* d, double = 0, double = 0, double = 0);
  std::string getColor() const;
  void setColor(double h, double s, double l);
  void update(double dt);
};

#endif  // DRONE_COLOR_DECORATOR_H
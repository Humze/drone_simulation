#include "DroneColorDecorator.h"

DroneColorDecorator::DroneColorDecorator(Drone* d, double h, double s, double l)
    : DroneDecorator(d), hue(h), saturation(s), light(l) {}

std::string DroneColorDecorator::getColor() const {
  auto sub_color = sub->getColor();
  double h, s, l;
  auto format = "hsl(%lf, %lf%%, %lf%%)";
  if (sscanf(sub_color.c_str(), format, &h, &s, &l) == 3) {
    h = (hue + h + 360 * (abs(hue - h) > 180)) / 2;
    if (h > 360) h -= 360;
    s = (saturation + s) / 2;
    l = (light + l) / 2;

  } else {
    h = hue;
    s = saturation;
    l = light;
  }
  char color[100];
  snprintf(color, sizeof(color), format, h, s, l);
  return color;
}

void DroneColorDecorator::setColor(double h, double s, double l) {
  hue = h;
  saturation = s;
  light = l;
}

// called every tick of the simulation
// this method was reworked with the help of claude ai
// specific prompts to ai with partial snippets of own created code, and the
// sentence "create debug statements", these have been removed to reduce bloat
// in cmd, give color combo code for purple (and the rest of the colors)
void DroneColorDecorator::update(double dt) {
  sub->update(dt);

  Package* p = nullptr;
  bool isLeaderDrone = false;
  bool isHelperDrone = false;

  if (LeaderDrone* leader = dynamic_cast<LeaderDrone*>(sub)) {
    isLeaderDrone = true;
    p = leader->getPackage();
  } else if (HelperDrone* helper = dynamic_cast<HelperDrone*>(sub)) {
    isHelperDrone = true;
    p = helper->getPackage();
  } else {
    // Generic fallback
    p = sub->getPackage();
  }

  if (p) {
    // Has a package - reuse gradient logic
    if (distToPackage < 0) {
      distToPackage = (sub->getPosition() - p->getPosition()).magnitude();
    }

    if (distToDestination < 0) {
      distToDestination = (p->getPosition() - p->getDestination()).magnitude();
    }

    if (sub->pickedUp) {
      // gradient from yellow to green
      double percentage =
          1 - ((sub->getPosition() - p->getDestination()).magnitude() /
               distToDestination);
      percentage = std::max(0.0, std::min(1.0, percentage));
      setColor(60 + 60 * percentage, 100, 50);
    } else if (!sub->pickedUp) {
      // gradient from red to yellow
      double percentage =
          1 -
          ((sub->getPosition() - p->getPosition()).magnitude() / distToPackage);
      percentage = std::max(0.0, std::min(1.0, percentage));
      setColor(60 * percentage, 100, 50);
    }
  } else {
    // No package
    distToPackage = -1;
    distToDestination = -1;

    if (!sub->available) {
      if (isLeaderDrone) {
        // to charging station
        setColor(270, 100, 50);  // Purple
      } else if (isHelperDrone) {
        // Helper unavailable w/o pickage
        setColor(300, 100, 50);  // Pink
      } else {
        // general
        setColor(270, 100, 50);  // Purple
      }
    } else {
      if (isLeaderDrone) {
        // leader available
        setColor(240, 100, 50);  // Blue
      } else if (isHelperDrone) {
        // helper avaialble
        setColor(180, 100, 50);  // Cyan
      } else {
        // General
        setColor(240, 100, 50);  // Blue
      }
    }
  }
}
Drone* DroneColorDecorator::getDecoratedEntity() const { return sub; }

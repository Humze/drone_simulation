#include "IPublisher.h"

#include <iostream>

void IPublisher::addObserver(const IObserver* o) { observers.insert(o); }

void IPublisher::addHelperDroneObserver(const IObserver* o) {
  // Add the new observer
  helperDroneObservers.insert(o);
  std::cout << helperDroneObservers.size() << std::endl;
}

void IPublisher::removeObserver(const IObserver* o) { observers.erase(o); }

void IPublisher::removeHelperDroneObserver(const IObserver* o) {
  helperDroneObservers.erase(o);
}

void IPublisher::notifyObservers(const std::string& message) {
  for (auto& o : observers) {
    o->notify(message);
  }
}

void IPublisher::notifyHelperDroneObservers(const std::string& message) {
  for (auto& helperDrones : helperDroneObservers) {
    helperDrones->notifyWithPublisher(message, this);
  }

  if (!helperDroneObservers.empty()) {
    // Use the first element from the set
    (*helperDroneObservers.begin())->notify(message);
  }
}

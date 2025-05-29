#ifndef IPUBLISHER_H_
#define IPUBLISHER_H_
#include <set>
#include <string>

#include "IObserver.h"

/**
 * @class IPublisher
 * @brief Follows the observer pattern from
 * https://refactoring.guru/design-patterns/observer. This defines the methods
 * that publishers implement and use
 *
 */
class IPublisher {
 public:
  /**
   * @brief adds observers to the set of observers
   * @param o type IObserver pointer
   */
  virtual void addObserver(const IObserver* o);
  /**
   * @brief adds helper drone observers to the set helperDroneObservers
   * @param o type IObserver pointer
   */
  virtual void addHelperDroneObserver(const IObserver* o);
  /**
   * @brief removes observers from the set observers
   * @param o type IObserver pointer
   */
  virtual void removeObserver(const IObserver* o);
  /**
   * @brief removes helper drone observer from the set helperDroneObservers
   * @param o type IObserver pointer
   */
  virtual void removeHelperDroneObserver(const IObserver* o);
  /**
   * @brief notifies the subscribed observers with a message, those in set of
   * observers
   * @param message of type string that updates the observers
   */
  virtual void notifyObservers(const std::string& message);
  /**
   * @brief Destructor for IPublisher
   */
  virtual ~IPublisher() = default;
  /**
   * @brief notifies the helperdrones subscribed with a message, those in set of
   * helperDroneObservers.
   * @param message of type string that updates the observers
   */
  virtual void notifyHelperDroneObservers(const std::string& message);

 private:
  std::set<const IObserver*> observers;
  std::set<const IObserver*> helperDroneObservers;
};

#endif  // IPUBLISHER_H_

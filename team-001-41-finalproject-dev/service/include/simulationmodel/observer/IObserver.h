#ifndef IOBSERVER_H_
#define IOBSERVER_H_

#include <string>

class IPublisher;
/**
 * @class IObserver
 * @brief Follows the observer pattern from
 * https://refactoring.guru/design-patterns/observer. This defines the methods
 * that observers will implement.
 */
class IObserver {
 public:
  /**
   * @brief Destructor for IObserver
   */
  virtual ~IObserver() = default;
  /**
   * @brief notify method that must be implemented by all observers.
   * @param message string that be sent as notification.
   */
  virtual void notify(const std::string& message) const = 0;
  /**
   * @brief notify with publisher method used by helperdrones to reply to
   * publishers
   * @param message string type that contains response to publisher
   * @param publisher type IPublisher* that the message is sent to
   */
  virtual void notifyWithPublisher(const std::string& message,
                                   IPublisher* publisher) const {}
};

#endif  // IOBSERVER_H_

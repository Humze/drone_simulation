#ifndef PACKAGE_H
#define PACKAGE_H

#include <vector>

#include "ExpeditedShipping.h"
#include "IEntity.h"
#include "NoRushShipping.h"
#include "PriorityShipping.h"
#include "StandardShipping.h"
#include "math/vector3.h"
#include "util/json.h"

class Robot;
/**
 * @class Package
 * @brief Represents a package in a physical system. Packages are delivered by
 * drones to robots
 */
class Package : public IEntity {
 public:
  /**
   * @brief Constructor
   * @param obj JSON object containing the package's information
   */
  Package(const JsonObject &obj);

  /**
   * @brief Gets the Package's destination
   * @return The Package's destination
   */
  virtual Vector3 getDestination() const;

  /**
   * @brief Returns the name of the strategy for this package
   *
   * @returns String name of strategy
   */
  virtual std::string getStrategyName() const;

  /**
   * @brief Returns the owner of the package
   *
   * @return pointer to Robot owning the package
   */
  virtual Robot *getOwner() const;

  /**
   * @brief Returns whether or not the package needs to be delivered
   *
   * @return boolean value of the above statement
   */
  virtual bool requiresDelivery() const;

  /**
   * @brief Set the Strategy Name
   *
   * @param strategyName_ Strategy name
   */
  virtual void setStrategyName(std::string strategyName_);

  /**
   * @brief Updates the Package
   *
   * @param dt difference in time since last update
   */
  virtual void update(double dt);

  /**
   * @brief Sets the attributes for delivery
   *
   * @param owner Robot for the package to be delivered to
   */
  virtual void initDelivery(Robot *owner);

  /**
   * @brief Gives the robot/owner this package
   */
  virtual void handOff();

  /**
   * @brief Sets the shipping priority for the package.
   *
   * @param newPriority Pointer to a PriorityShipping strategy.
   */
  void setPriority(PriorityShipping *newPriority);

  /**
   * @brief Gets the current shipping priority object.
   *
   * @return Pointer to the current PriorityShipping strategy.
   */
  PriorityShipping *getPriority() const;

  /**
   * @brief Gets the name of this shipping priority.
   *
   * @return String of the priority's name. If priority wasn't set up
   * originally, returns the string Standard.
   */
  std::string getPriorityName() const;

  /**
   * @brief Gets the integer value of the priority level in this shipping
   * method.
   *
   * @return An integer that represents the priority level. If priority wasn't
   * set  up originally, returns the integer 1.
   */
  int getPriorityLevel() const;

  /**
   * @brief Checks whether the package has been scheduled for delivery.
   *
   * @return true if the package is scheduled, false otherwise.
   */
  bool isScheduled() const { return scheduled; }

  /**
   * @brief Sets the scheduled status of the package.
   *
   * @param val Boolean indicating whether the package is scheduled.
   */
  void setScheduled(bool val) { scheduled = val; }

  /**
   * @brief Marks the package as picked up.
   */
  void isPickedUp();

  /**
   * @brief Checks if the package has been picked up.
   *
   * @return true if the package has been picked up, false otherwise.
   */
  bool getPackagePickedUp() const;

  /**
   * @brief Marks the package as delivered.
   */
  void DeliveredPackage();

  /**
   * @brief Checks if the package has been delivered.
   *
   * @return true if the package has been delivered, false otherwise.
   */
  bool getDeliveredPackage() const;

 protected:
  bool requiresDelivery_ = true;
  Vector3 destination;
  std::string strategyName;
  Robot *owner = nullptr;
  PriorityShipping *priority = nullptr;
  bool pickedUp = false;
  bool isDelivered = false;
  bool scheduled = false;
};

#endif  // PACKAGE_H

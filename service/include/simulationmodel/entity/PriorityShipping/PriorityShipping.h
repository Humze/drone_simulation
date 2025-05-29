#ifndef PRIORITY_SHIPPING_H_
#define PRIORITY_SHIPPING_H_

#include <string>

/**
 * @class PriorityShipping
 * @brief Represents Priority shipping option in new package priority feature.
 *
 */
class PriorityShipping {
 public:
  /**
   * @brief Destructor for deletion.
   */

  virtual ~PriorityShipping() {}

  /**
   * @brief Gets the name of the shipping priority.
   *
   * @return A string representing the priority type. Either Expedited, Standard
   * or No Rush.
   */

  virtual std::string getPriorityName() const = 0;

  /**
   * @brief Gets the integer value of the priority level in this shipping
   * method.
   *
   * @return An integer that represents the priority level.
   */

  virtual int getPriorityLevel() const = 0;  // Lower number = higher priority
};

#endif  // PRIORITY_SHIPPING_H_

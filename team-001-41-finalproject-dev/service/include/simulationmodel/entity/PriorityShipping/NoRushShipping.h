#ifndef NO_RUSH_SHIPPING_H_
#define NO_RUSH_SHIPPING_H_

#include "PriorityShipping.h"
/**
 * @class NoRushShipping
 * @brief Represents No Rush shipping option in new package priority feature.
 *
 */
class NoRushShipping : public PriorityShipping {
 public:
  /**
   * @brief Gets the name of this shipping priority.
   *
   * @return String of the priority's name.
   */

  std::string getPriorityName() const;

  /**
   * @brief Gets the integer value of the priority level in this shipping
   * method.
   *
   * @return An integer that represents the priority level.
   */

  int getPriorityLevel() const;
};

#endif  // NO_RUSH_SHIPPING_H_

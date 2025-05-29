#ifndef NO_RUSH_SHIPPING_H_
#define NO_RUSH_SHIPPING_H_

#include "PriorityShipping.h"

class NoRushShipping : public PriorityShipping {
public:
    std::string getPriorityName() const  ;
    int getPriorityLevel() const  ;
};

#endif // NO_RUSH_SHIPPING_H_

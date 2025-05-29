#ifndef EXPEDITED_SHIPPING_H_
#define EXPEDITED_SHIPPING_H_

#include "PriorityShipping.h"

class ExpeditedShipping : public PriorityShipping {
public:
    std::string getPriorityName() const ;
    int getPriorityLevel() const ;
};

#endif // EXPEDITED_SHIPPING_H_

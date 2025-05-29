#ifndef STANDARD_SHIPPING_H_
#define STANDARD_SHIPPING_H_

#include "PriorityShipping.h"

class StandardShipping : public PriorityShipping {
public:
    std::string getPriorityName() const  ;
    int getPriorityLevel() const  ;
};

#endif // STANDARD_SHIPPING_H_

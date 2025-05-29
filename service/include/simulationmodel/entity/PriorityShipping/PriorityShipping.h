#ifndef PRIORITY_SHIPPING_H_
#define PRIORITY_SHIPPING_H_

#include <string>

class PriorityShipping {
public:
    
    virtual std::string getPriorityName();
    virtual int getPriorityLevel();
    virtual ~PriorityShipping() {}
};

#endif // PRIORITY_SHIPPING_H_

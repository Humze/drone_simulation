#include "NoRushShipping.h"

std::string NoRushShipping::getPriorityName() const {
    return "No Rush";
}

int NoRushShipping::getPriorityLevel() const {
    return 2;
}

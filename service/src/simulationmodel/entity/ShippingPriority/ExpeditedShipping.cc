#include "ExpeditedShipping.h"

std::string ExpeditedShipping::getPriorityName() const {
    return "Expedited";
}

int ExpeditedShipping::getPriorityLevel() const {
    return 0;
}

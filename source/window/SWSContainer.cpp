#include "SWSContainer.hpp"

void SWSContainer::addChild(SWSWidget *child, unsigned long index) {
    _children.insert(child, index);
    _amountOfChildren++;
}

#include "SWSContainer.hpp"

void SWSContainer::addChild(SWSWidget *child, unsigned long index) {
    _children.insert(child, index);
    _amountOfChildren++;
}

void procCallback(SWSWidget* child){
    child->proc();
}

void SWSContainer::proc() {
    SWSWidget::proc();
    _children.forEach(&procCallback);
}
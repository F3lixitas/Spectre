#include "SWSContainer.hpp"

void SWSContainer::addChild(SWSWidget *child, unsigned long index) {
    _children.insert(child, index);
    _amountOfChildren++;
}

#if defined __linux__ || defined __APPLE__

void procCallback(SWSWidget* child, xcb_generic_event_t* event){
    child->proc(event);
}

void SWSContainer::proc(xcb_generic_event_t* event) {
    SWSWidget::proc(event);
    _children.forEach<xcb_generic_event_t*>(&procCallback, event);
}
#endif
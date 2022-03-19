#ifndef SPECTRE_INDEXED_LIST
#define SPECTRE_INDEXED_LIST

template<typename T>
struct __indexedListBody{
    T* element;
    unsigned long       index;
    __indexedListBody*  next;
};

template<typename T>
class SIndexedList {
private:
    unsigned int           _numberOfElements;
    __indexedListBody<T>*  _firstElement;
public:
    void insert(T* element, unsigned long index){
        __indexedListBody<T> *body = new __indexedListBody<T>;
        body->element = element;
        body->index = index;
        body->next = _firstElement;
        _firstElement = body;
        _numberOfElements ++;
    }

    SIndexedList(){
        _numberOfElements = 0;
        _firstElement = nullptr;
    }
    SIndexedList(T* init, unsigned long index){
        _numberOfElements = 0;
        _firstElement = nullptr;
        insert(init, index);
    }
    ~SIndexedList(){
        __indexedListBody<T>* currentElement = _firstElement;
        while(currentElement != nullptr){
            _firstElement = currentElement->next;
            delete currentElement;
            currentElement = _firstElement;
        }
    }

    void deleteElementByIndex(unsigned long index){
        __indexedListBody<T>* currentElement = _firstElement;
        __indexedListBody<T>* previousElement = nullptr;
        for(unsigned int i; i < _numberOfElements; i++){
               if(currentElement->index == index){
                   if(previousElement == nullptr) _firstElement = currentElement->next;
                   previousElement->next = currentElement->next;
                   delete currentElement->element;
                   delete currentElement;
                   return;
               }
               previousElement = currentElement;
               currentElement = currentElement->next;
        }
    }


    void forEach(void (*callback)(T*)){
        __indexedListBody<T>* currentElement = _firstElement;
        for(unsigned int i; i < _numberOfElements; i++){
            callback(currentElement->element);
            currentElement = currentElement->next;
        }
    }
    template<typename G>
    void forEach(void (*callback)(T*, G), G arg1){
        __indexedListBody<T>* currentElement = _firstElement;
        for(unsigned int i = 0; i < _numberOfElements; i++){
            callback(currentElement->element, arg1);
            currentElement = currentElement->next;
        }
    }

};

#endif
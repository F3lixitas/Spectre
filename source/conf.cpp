#include "conf.hpp"

bool updated = false;
SIndexedList<SLog>* logs = new SIndexedList<SLog>;

void sAddLog(SLog log){
    static unsigned long i = 0;
    SLog* l = new SLog;
    l->text = log.text;
    l->type = log.type;
    logs->insert(l, i);
    i++;
    if(logs->size() > LOG_BUFFER_SIZE)
        logs->deleteLast();
    updated = true;
}
bool sNewLog(){
    if(updated){
        updated = false;
        return true;
    } else return false;
}
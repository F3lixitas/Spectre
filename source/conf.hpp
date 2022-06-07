#ifndef SPECTRE_CONF
#define SPECTRE_CONF

///put application settings here

/// SPECTRE_ENGINE or SPECTRE_CORE (not implemented yet)
#define SPECTRE_ENGINE
#include "logging/Errors.hpp"
#ifdef SPECTRE_ENGINE
    /// Engine's logging system
    #define LOG_BUFFER_SIZE 100
    #include "utils/SIndexedList.hpp"

    extern SIndexedList<SLog>* logs;

    void sAddLog(SLog log);
    bool sNewLog();

    #define ASSERT(log) if(log.type != S_LOG_SUCCESS) sAddLog(log)
#endif
#endif
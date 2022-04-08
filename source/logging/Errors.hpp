#ifndef SPECTRE_ERRORS
#define SPECTRE_ERRORS

#include <string>

typedef enum{
    S_LOG_SUCCESS,
    S_LOG_INFO,
    S_LOG_WARNING,
    S_LOG_ERROR,
    S_LOG_CRITICAL
}SLogType;

typedef struct SLog{
    std::wstring text;
    SLogType type;
}SLog;

#endif
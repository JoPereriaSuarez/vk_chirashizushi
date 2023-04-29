#include <stdio.h>

typedef enum LOG_LEVEL_T
{
    LOG_VERBOSE = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3,
    LOG_NONE = 4
}LOG_LEVEL;

#ifndef LOG_CURRENT_LEVEL
#define LOG_CURRENT_LEVEL LOG_WARNING
#endif

#define LOG_MESSAGE(LEVEL, MESSAGE) \
({if(LEVEL >= LOG_CURRENT_LEVEL) \
    printf("[%s]%s\n", log_to_str(LEVEL), MESSAGE);})
char *log_to_str(LOG_LEVEL log_level)
{
    switch (log_level)
    {
        case LOG_VERBOSE:
            return "VERBOSE"; 
        case LOG_INFO:
            return "INFO"; 
        case LOG_WARNING:
            return "WARNING"; 
        case LOG_ERROR:
            return "ERROR";    
    }
}

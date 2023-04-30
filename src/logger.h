#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>

#ifndef LOG_CURRENT_LEVEL
typedef enum LOG_LEVEL_T
{
    LOG_VERBOSE = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3,
    LOG_NONE = 4
}LOG_LEVEL;
#define LOG_CURRENT_LEVEL LOG_WARNING
#define LOG_LEVEL_TO_STR(LEVEL) (LEVEL == LOG_VERBOSE ? "VERBOSE": \
    LEVEL == LOG_INFO ? "INFO": \
    LEVEL == LOG_WARNING ? "WARNING":\
    LEVEL == LOG_ERROR ? "ERROR": "NONE")

#define LOG_MESSAGE(LEVEL, MESSAGE) \
({if(LEVEL >= LOG_CURRENT_LEVEL) \
    printf("[%s] %s\n", LOG_LEVEL_TO_STR(LEVEL), MESSAGE);})

#endif

#endif /*_LOGGER_H_ */
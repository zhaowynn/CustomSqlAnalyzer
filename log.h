#ifndef _LOG_H_
#define _LOG_H_

#include <syslog.h>


#define LOG(level, fmt, ...) \
    do { syslog(level, fmt"  (%s:%d:%s)\n",##__VA_ARGS__,__FILE__,__LINE__,__FUNCTION__); } while(0);

#endif

// LogManager.h

#ifndef _LOG_MANAGER_H_INCLUDED_
#define _LOG_MANAGER_H_INCLUDED_

#include "Log.h"

struct LogList;
struct LogList
{
    Log *log;
    unsigned int type;
    LogList *next;
};

class LogManager
{
public:
    LogManager();
    ~LogManager();
    
    void Check();
    void Add(Log *log);
    void Remove(Log *log);

private:
    LogList *m_Logs;
    Mutex m_Mutex;
};

#endif // _LOG_MANAGER_H_INCLUDED_

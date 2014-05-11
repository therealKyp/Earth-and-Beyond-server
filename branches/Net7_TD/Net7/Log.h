// Log.h

#ifndef _LOG_H_INCLUDED_
#define _LOG_H_INCLUDED_

#include "MessageQueue.h"

#define PRINT_BUFFER_SIZE 256

class Log
{
public:
    Log();
    Log(const char *file);
    virtual ~Log();
    
    void SetFileName(const char *file);
    void Flush();
    void Print(char *format, ...);

private:
    char *m_FileName;
    FILE *m_FilePtr;
    MessageQueue m_Queue;
};

#endif // _LOG_H_INCLUDED_

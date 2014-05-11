// Log.cpp

#include "Net7.h"
#include "Log.h"
#include "LogManager.h"

Log::Log()
{
    m_FilePtr = NULL;
    m_FileName = NULL;
    
    g_LogManager.Add(this);
}

Log::Log(const char *file)
{
    int size;
    
    size = strlen(file) + strlen(SERVER_LOGS_PATH) + 1;
    
    m_FileName = new char[size];
    
    strcpy(m_FileName, SERVER_LOGS_PATH);
    
    strcat(m_FileName, file);
    
    m_FilePtr = fopen(m_FileName, "a");
    
    /* If the file failed to open... */
    if(m_FilePtr == NULL)
    {
        int error = errno;
        fprintf(stderr, "Log::Log()\tERROR: Failed to open file %s. %s.\n", m_FileName, strerror(error));
        delete [] m_FileName;
        m_FileName = NULL;
    }
    
    g_LogManager.Add(this);
}

Log::~Log()
{
    if(m_FileName)
        delete [] m_FileName;
        
    if(m_FilePtr)
        fclose(m_FilePtr);
        
    g_LogManager.Remove(this);
    
    /* Need to figure out how to deal with the message queue
       still having entries that haven't been deleted yet. */
}

void Log::SetFileName(const char *file)
{
    if(m_FileName)
        delete [] m_FileName;
    
    if(m_FilePtr)
        fclose(m_FilePtr);
    
    int size = strlen(file) + sizeof(SERVER_LOGS_PATH) + 1;
    
    m_FileName = new char[size];
    
    strcpy(m_FileName, SERVER_LOGS_PATH);
    
    strcat(m_FileName, file);
    
    m_FilePtr = fopen(m_FileName, "a");
    
    /* If the file failed to open... */
    if(m_FilePtr == NULL)
    {
        int error = errno;
        fprintf(stderr, "Log::Log()\tERROR: Failed to open file %s. %s.\n", m_FileName, strerror(error));
        delete [] m_FileName;
        m_FileName = NULL;
    }
}

void Log::Flush()
{
    char *msg = NULL;
    
    if(m_FileName == NULL || m_FilePtr == NULL)
        return;
    
    while(m_Queue.CheckQueue(&msg))
    {
        fputs(msg, m_FilePtr);
        fputs(msg, stdout);
        delete [] msg;
    }
    fflush(m_FilePtr);
}

void Log::Print(char *format, ...)
{
    char buffer1[PRINT_BUFFER_SIZE];
    char buffer2[PRINT_BUFFER_SIZE];
    time_t current_time;
    struct tm *timeinfo;
    
    va_list args;
    va_start(args, format);
    vsnprintf(buffer1, PRINT_BUFFER_SIZE, format, args);
    va_end(args);
    
    current_time = time(NULL);
    timeinfo = localtime(&current_time);
    
    snprintf(buffer2, PRINT_BUFFER_SIZE, "(%.4d/%.2d/%.2d %.2d:%.2d:%.2d) %s\n", 
        timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
        timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, buffer1);
        
    m_Queue.Add(buffer2);
}

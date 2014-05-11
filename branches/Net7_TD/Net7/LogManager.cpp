// LogManager.cpp

#include "Net7.h"
#include "LogManager.h"

LogManager::LogManager()
{
    m_Logs = NULL;
}

LogManager::~LogManager()
{
    LogList *current, *next;
    
    current = m_Logs;
    next = NULL;
    
    while(current != NULL)
    {
        next = current->next;
        //delete current->log;
        delete current;
        current = next;
    }
    
    m_Logs = NULL;
}

void LogManager::Check()
{
    m_Mutex.Lock();
    
    LogList *current;
    
    current = m_Logs;
    
    while(current != NULL)
    {
        current->log->Flush();
        current = current->next;
    }
    
    m_Mutex.Unlock();
}

void LogManager::Add(Log *log)
{
    LogList *current;

    m_Mutex.Lock();
    
    current = m_Logs;
    
    if(current == NULL)
    {
        m_Logs = new LogList;
        
        m_Logs->log = log;
        m_Logs->next = NULL;
    }
    else
    {
        // Look for the last log in the list.
        while(current->next != NULL)
            current = current->next;
        
        current->next = new LogList;
        current->next->log = log;
        current->next->next = NULL;
    }
    
    m_Mutex.Unlock();
}

void LogManager::Remove(Log *log)
{
    LogList *current, *previous;

    m_Mutex.Lock();
    
    current = m_Logs;
    previous = NULL;
    
    while(current != NULL)
    {
        if(current->log != log && current->next != NULL)
        {
            previous = current;
            current = current->next;
        }
        else if(current->log == log)
        {
            if(previous != NULL)
                previous->next = current->next;

            if(current == m_Logs)
                m_Logs = current->next;

            delete current;
            
            return;
        }
        else if(current->next == NULL)
        {
            return;
        }
    }
    
    m_Mutex.Unlock();
}

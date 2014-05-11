// Mutex.cpp
//
// Encapsulates mutual exclusion methods for both Windows and Linux platforms.
//
// DESIGN NOTES:
// Windows allows nested calls to the mutex lock method, but Linux does not.
// This class has been modified to add a "Lock Count" to pthreads_mutex in order
// to support nested calls.
//
#include "Net7.h"
#include "Mutex.h"

Mutex::Mutex()
{
#ifdef WIN32
    InitializeCriticalSection(&m_Mutex);
#else
    pthread_mutex_init(&m_Mutex, NULL);
    m_LockCount = 0;
#endif
}

Mutex::~Mutex()
{
#ifdef WIN32
    DeleteCriticalSection(&m_Mutex);
#else
    pthread_mutex_destroy(&m_Mutex);
#endif
}

void Mutex::Lock()
{
#ifdef WIN32
    EnterCriticalSection(&m_Mutex);
#else
    pthread_t thread_id = pthread_self();
    int ret = pthread_mutex_trylock(&m_Mutex);
    if (ret == 0)
    {
        // Lock was successful
        m_ThreadID = thread_id;
        m_LockCount++;
    }
    else if (m_ThreadID == thread_id)
    {
        // Lock failed, we already own the lock
        m_LockCount++;
    }
    else
    {
        // Another thread owns the lock.
        // We have no choice but to wait until they release it.
        pthread_mutex_lock(&m_Mutex);
    }
#endif
}

void Mutex::Unlock()
{
#ifdef WIN32
    LeaveCriticalSection(&m_Mutex);
#else
    if (m_LockCount > 0)
    {
        m_LockCount--;
        if (m_LockCount == 0)
        {
            // When the lock count reaches zero, release the resource
            pthread_mutex_unlock(&m_Mutex);
        }
    }
    // else mutex is not locked, ignore the call
#endif
}


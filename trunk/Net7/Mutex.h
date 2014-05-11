// Mutex.h

#ifndef _MUTEX_H_INCLUDED_
#define _MUTEX_H_INCLUDED_

#ifdef WIN32
    #ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
    #endif

    #include <windows.h>
#else
    #include <pthread.h>
#endif

class Mutex
{
public:
    Mutex();
    virtual ~Mutex();

public:
    void Lock();
    void Unlock();

private:
#ifdef WIN32
    CRITICAL_SECTION m_Mutex;
#else
    pthread_mutex_t m_Mutex;
    pthread_t m_ThreadID;
    int m_LockCount;
#endif
};

#endif // _MUTEX_H_INCLUDED_

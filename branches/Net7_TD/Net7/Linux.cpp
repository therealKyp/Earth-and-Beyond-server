// Linux.cpp

#ifndef WIN32

#include "Net7.h"
#include "Linux.h"

unsigned long GetTickCount()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

unsigned long GetCurrentDirectory(unsigned long size, char *path)
{
    if (getcwd(path, size) < 0)
    {
        return 0;
    }
    return (strlen(path));
}

int SetCurrentDirectory(char *path)
{
    if (chdir(path) < 0)
    {
        return 0;
    }
    return 1;
}

void Sleep(unsigned long dwMilliseconds)
{
    usleep((unsigned int) dwMilliseconds * 1000);
}

bool DeleteFile(const char *file)
{
    return (!remove(file));
}

#endif

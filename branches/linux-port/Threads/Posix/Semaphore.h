/////////////////////////////////////////////////////////////////////
//  Written by Phillip Sitbon
//  Copyright 2003
//
//  Posix/Semaphore.h
//    - Resource counting mechanism
//
/////////////////////////////////////////////////////////////////////

#ifndef _Semaphore_Posix_
#define _Semaphore_Posix_

#include <semaphore.h>

class Semaphore
{
  sem_t S;

  public:
  Semaphore( unsigned int init = 0 )
  { int ret = sem_init(&S,0,init);
	  if(ret != 0)
	  {
		  printf("Failed to init semaphore with error: %d", (errno));
	  }
  }

  virtual ~Semaphore()
  { sem_destroy(&S); }

  void Wait() const
  { sem_wait((sem_t *)&S); }

  int Wait_Try() const
  { return (sem_trywait((sem_t *)&S)?errno:0); }

  int Post() const
  { return (sem_post((sem_t *)&S)?errno:0); }

  int Value() const
  { int V = -1; sem_getvalue((sem_t *)&S,&V); return V; }

  void Reset( int init = 0 )
  { sem_destroy(&S); sem_init(&S,0,init); }
};

#endif // !_Semaphore_Posix_

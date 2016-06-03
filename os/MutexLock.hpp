#ifndef OS_MUTEXLOCK_HPP
#define OS_MUTEXLOCK_HPP

#include "fosi.h"
#include "Mutex.hpp"
namespace RTT
{
  namespace os
  {
    class MutexLock
    {
      public:
      MutexLock(MutexInterface &mutex)
      {
        _mutex = &mutex;
        _mutex->lock();
      }

      ~MutexLock()
      {
        _mutex->unlock();
      }
      protected:
        MutexInterface *_mutex;
        MutexLock(){}
    };

    class MutexTryLock
    {
    public:
      MutexTryLock(MutexInterface &mutex):_mutex(&mutex),successful(mutex.trylock())
      {

      }
    protected:
      MutexInterface *_mutex;
      MutexTryLock(){}
    private:
      bool successful;
    };

    class MutexTimedLock
    {
    public:
      MutexTimedLock(MutexInterface &mutex,Seconds timeout)
        :_mutex(&mutex),successful(mutex.timedlock(timeout))
      {

      }
      bool isSuccessful()
      {
        return successful;
      }
    protected:
      MutexInterface *_mutex;
      MutexTimedLock(){}
    private:
      bool successful;
    };
  }
}
#endif

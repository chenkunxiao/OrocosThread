#ifndef OS_MUTEX_HPP
#define OS_MUTEX_HPP

#include "fosi.h"
#include "Time.hpp"
//#include "rtt-os-fwd.hpp"
#ifndef ORO_OS_USE_BOOST_THREAD
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#endif

namespace RTT
{
  namespace os
  {
    class MutexInterface
    {
    public:
      virtual ~MutexInterface(){}
      virtual void lock()=0;
      virtual void unlock()=0;
      virtual bool trylock()=0;
      virtual bool timedlock(Seconds s)=0;
    };

    class Mutex:public MutexInterface
    {
    friend class Condition;
#ifndef ORO_OS_USE_BOOST_THREAD
     protected:
         rt_mutex_t m;
    public:
      Mutex()
      {
        rtos_mutex_init(&m);
      }

      virtual ~Mutex()
      {
        if(trylock())
          {
            unlock();
            rtos_mutex_destroy(&m);
          }
      }

      virtual void lock()
      {
        rtos_mutex_lock(&m);
      }
      virtual void unlock()
      {
        rtos_mutex_unlock(&m);
      }
      virtual bool trylock()
      {
        if(rtos_mutex_trylock(&m) == 0)
          return true;

        return false;
      }
      virtual bool timedlock(Seconds s)
      {
        if(rtos_mutex_lock_until( &m, rtos_get_time_ns() + Seconds_to_nsecs(s) ) == 0)
          return true;

        return false;
      }
#else
     protected:
         boost::timed_mutex m;
    public:
      Mutex()
      {
      }
      virtual ~Mutex()
      {
      }
      virtual void lock()
      {
        m.lock();
      }
      virtual void unlock()
      {
        m.unlock();
      }
      virtual bool trylock()
      {
        return m.try_lock();
      }
      virtual bool timedlock(Seconds s)
      {
        return m.timed_lock( boost::posix_time::microseconds(Seconds_to_nsecs(s)/1000) );
      }
#endif
    };

    class MutexRecursive:public MutexInterface
    {
      #ifndef ORO_OS_USE_BOOST_THREAD
    protected:
      rt_rec_mutex_t recm;
    public:
      MutexRecursive()
      {
        rtos_mutex_rec_init(&recm);
      }
      virtual ~MutexRecursive()
      {
        if(trylock())
        {
          unlock();
          rtos_mutex_rec_destroy(&recm);
        }
      }
      void lock()
      {
        rtos_mutex_rec_lock(&recm);
      }

      virtual void unlock()
      {
        rtos_mutex_rec_unlock(&recm);
      }

      virtual bool trylock()
      {
        if(rtos_mutex_rec_trylock(&recm) == 0)
          return true;
        return false;
      }

      virtual bool timedlock(Seconds s)
      {
        if ( rtos_mutex_rec_lock_until( &recm, rtos_get_time_ns() + Seconds_to_nsecs(s) ) == 0 )
          return true;

        return false;
      }
#else
    protected:
      boost::recursive_timed_mutex recm;
    public:
      MutexRecursive()
      {
      }

      virtual ~MutexRecursive()
      {
      }

      void lock()
      {
        recm.lock();
      }

      virtual void unlock()
      {
        recm.unlock();
      }

      virtual bool trylock()
      {
        return recm.try_lock();
      }

      virtual bool timedlock(Seconds s)
      {
        return recm.timed_lock( boost::posix_time::microseconds( Seconds_to_nsecs(s)/1000 ) );
      }
#endif
    };
  }
}
#endif

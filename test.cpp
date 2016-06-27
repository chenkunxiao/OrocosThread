#include "os/fosi.h"
#include <stdio.h>
#include <boost/scoped_ptr.hpp>
#include <iostream>
//#include "os/Thread.hpp"
//#include "os/ThreadInterface.hpp"
//#include "os/threads.hpp"
//#include "base/ActivityInterface.hpp"
#include "Activity.hpp"
//#include "internal/List.hpp"
#include "ExecutionEngine.hpp"
//#include "internal/BindStorage.hpp"
//#include "internal/LocalOperationCaller.hpp"
//#include "rtt-fwd.hpp"
#include "OperationCaller.hpp"
#include "OperationInterfacePart.hpp"
#include "OperationInterface.hpp"
#include "types/TypeInfo.hpp"
#include "InputPort.hpp"
#include "OutputPort.hpp"
using namespace std;
//#include <boost/test/included/unit_test.hpp>
#define QS 10
using namespace RTT;
using namespace RTT::internal;
using namespace RTT::base;


class Dummy {
public:
    Dummy(double a = 0.0, double b =1.0, double c=2.0)
        :d1(a), d2(b), d3(c) {}
    double d1;
    double d2;
    double d3;
    bool operator==(const Dummy& d) const
    {
        return d.d1 == d1 && d.d2 == d2 && d.d3 == d3;
    }

    bool operator!=(const Dummy& d) const
    {
        return d.d1 != d1 || d.d2 != d2 || d.d3 != d3;
    }

    bool operator<(const Dummy& d) const
    {
        return d1+d2+d3 < d.d1 + d.d2 + d.d3;
    }
};

class BuffersDataFlowTest
{
public:
    BufferInterface<Dummy>* buffer;
    BufferInterface<Dummy>* circular;
    DataObjectInterface<Dummy>* dataobj;

    BufferLockFree<Dummy>* lockfree;
    BufferLocked<Dummy>* locked;

    BufferLockFree<Dummy>* clockfree;
    BufferLocked<Dummy>* clocked;

    DataObjectLocked<Dummy>* dlocked;
    DataObjectLockFree<Dummy>* dlockfree;

    void testBuf();
    void testCirc();
    void testDObj();

    BuffersDataFlowTest()
    {
      // clasical variants
      lockfree = new BufferLockFree<Dummy>(QS);
      locked = new BufferLocked<Dummy>(QS);

        // circular variants.
      clockfree = new BufferLockFree<Dummy>(QS,Dummy(), true);
      clocked = new BufferLocked<Dummy>(QS,Dummy(), true);

      dlockfree = new DataObjectLockFree<Dummy>();
      dlocked   = new DataObjectLocked<Dummy>();

        // defaults
      buffer = lockfree;
      dataobj = dlockfree;
    }

    ~BuffersDataFlowTest(){
      delete lockfree;
      delete locked;
      //delete unsync;
      delete clockfree;
      delete clocked;
      //delete cunsync;
      delete dlockfree;
      delete dlocked;
      //delete dunsync;
    }
};

void BuffersDataFlowTest::testBuf()
{
    Dummy* d = new Dummy;
    Dummy* c = new Dummy(2.0, 1.0, 0.0);
    Dummy r;
    buffer->Push( *d ) ;
    buffer->Push( *c ) ;
    buffer->Push( *d ) ;
    buffer->Push( *c ) ;

    buffer->Pop(r);
    if(r == *d)
      {
        cout << "1" << endl;
      }
    buffer->Pop(r);
    if(r == *c)
      {
        cout << "2" << endl;
      }
     buffer->Pop(r);
     if(r == *d)
      {
        cout << "3" << endl;
      }
    buffer->Pop(r);
    if(r == *c)
      {
        cout << "4" << endl;
      }

    buffer->Push( *c );
    buffer->Push( *d );
    buffer->Push( *c );
    buffer->Push( *d );
    buffer->Push( *c );

    std::vector<Dummy> v;
    buffer->Pop(v);

    if(v[0] == *c)
      {
        if(v[1] == *d)
          {
            if(v[2] == *c)
              {
                if(v[3] == *d)
                  {
                    if(v[4] == *c)
                      {
                        cout << "Successful" << endl;
                      }
                  }
              }
          }
      }
    // buffer->Pop(r);
    //buffer->Push( *d );
    delete d;
    delete c;
}

class BuffersMPoolTest
{
public:
  TsPool<Dummy>* mpool;
  TsPool<std::vector<Dummy> >* vpool;

  BuffersMPoolTest()
  {
    mpool = new TsPool<Dummy>(QS);
    vpool = new TsPool<std::vector<Dummy> >(QS, std::vector<Dummy>(QS) );
  }

  ~BuffersMPoolTest(){
    delete mpool;
    delete vpool;
  }
};

int main()
{
 // ExecutionEngine* ee = new ExecutionEngine;
  //Activity m_activity(1,1,ee,"first");
  // m_activity.start();
  //while(1);
  // OperationCaller<int> opc0r("sss",ee);
  //OperationCaller<double(double)> mychange("sss",ee);
  int value = 0;
  OutputPort<int> wp("W");
  InputPort<int> rp1("R1", ConnPolicy::data());
  InputPort<int> rp2("R2", ConnPolicy::buffer(4));
  InputPort<int> rp3("R3", ConnPolicy::data());
  wp.createConnection(rp1);
  wp.createConnection(rp2);
  wp.createConnection(rp3);

  wp.write(10);
  wp.write(15);
  wp.write(20);
  wp.write(25);
  wp.write(30);

  rp1.read(value);
  cout << value << endl;

  rp2.read(value);
  cout << value << endl;

  rp2.read(value);
  cout << value << endl;

  rp2.read(value);
  cout << value << endl;

  rp2.read(value);
  cout << value << endl;

  rp3.read(value);
  cout << value << endl;

  return 1;
}

#include "os/fosi.h"
#include <stdio.h>
#include <boost/scoped_ptr.hpp>
#include <iostream>
//#include "os/Thread.hpp"
//#include "os/ThreadInterface.hpp"
//#include "os/threads.hpp"
//#include "base/ActivityInterface.hpp"
//#include "Activity.hpp"
//#include "internal/List.hpp"
//#include "ExecutionEngine.hpp"
//#include "internal/BindStorage.hpp"
//#include "internal/LocalOperationCaller.hpp"
//#include "rtt-fwd.hpp"
#include "OperationCaller.hpp"
//#include "OperationInterfacePart.hpp"
//#include "OperationInterface.hpp"
//#include "types/TypeInfo.hpp"
#include "InputPort.hpp"
#include "OutputPort.hpp"
#include "TaskContext.hpp"
#include "Service.hpp"
using namespace std;
#define ORO_TEST_OPERATION_CALLER 1
//#define OS_RT_MALLOC 1
//#include <boost/test/included/unit_test.hpp>
#define QS 10
using namespace RTT;
using namespace RTT::internal;
using namespace RTT::base;


double ret;
double& m0r() { return ret; }
const double& m0cr() { return ret; }

// test const std::string& argument for command_ds
bool comstr(const std::string& cs) { return !cs.empty(); }

double m1r(double& a) { a = 2*a; return a; }
double m1cr(const double& a) { return a; }

// plain argument tests:
void   vm0(void) { return; }
double m0(void) { return -1.0; }
double m1(int i) { if (i ==1) return -2.0; else return 2.0; }
double m2(int i, double d) { if ( i == 1 && d == 2.0 ) return -3.0; else return 3.0; }
double m3(int i, double d, bool c) { if ( i == 1 && d == 2.0 && c == true) return -4.0; else return 4.0; }
double m4(int i, double d, bool c, std::string s) { if ( i == 1 && d == 2.0 && c == true && s == "hello") return -5.0; else return 5.0;  }
double m5(int i, double d, bool c, std::string s, float f) { if ( i == 1 && d == 2.0 && c == true && s == "hello" && f == 5.0f) return -6.0; else return 6.0;  }
double m6(int i, double d, bool c, std::string s, float f, char h) { if ( i == 1 && d == 2.0 && c == true && s == "hello" && f == 5.0f && h == 'a') return -7.0; else return 7.0;  }
double m7(int i, double d, bool c, std::string s, float f, char h, unsigned int st) { if ( i == 1 && d == 2.0 && c == true && s == "hello" && f == 5.0f && h == 'a', st == 7) return -8.0; else return 8.0;  }

//exception tests:
void m0except(void) { throw std::runtime_error("exception"); }

void print(const std::string& what) { cout << "print: " << what <<endl; }
void printNumber(const std::string& what, int n) { cout << "print: " << what << n << endl; }

bool fail() {
  throw false;
  return true; // should be ignored anyway.
}

bool good() {
  return true;
}

bool assertBool(bool b) {
  if (!b) throw b;
  return b;
}

bool isTrue(bool b) {
  return b;
}

bool assertEqual(int a, int b)
{
  if (a != b) {
    cerr << "AssertEqual failed: a != b " << a << " != " << b << "." << endl;
    throw b;
  }
  return a == b;
}
bool assertMsg( bool b, const std::string& msg) {
  if ( b == false ) {
    cout << "Asserted :" << msg << endl;
    throw b;
  }
  return true; // allow to continue to check other commands.
}
int i;
int increase() { return ++i;}
void resetI() { i = 0; }
int getI() { return i; }

int main()
{
  double ret = 0.0;
  TaskContext* caller = new TaskContext("caller");
  TaskContext* tc = new TaskContext("root");
  Service::shared_ptr dat = Service::Create("test",tc);
  tc->provides()->addAttribute("ret",ret);
  tc->start();
  caller->start();
  dat->addOperation("m0",&m0);
  dat->addOperation("m1",&m1);
  dat->addOperation("m2",&m2);
  dat->addOperation("m3",&m3);
  // OperationCaller<double(void)> mm0("m0");
  // mm0 = tc->provides("test");
  // OperationCaller<double(int)> mm1(tc->provides("test")->getOperation("m1"));
  // OperationCaller<double(int,double)> mm2( tc->provides("test")->getOperation("m2"));
  // OperationCaller<double(int,double,bool)> mm3( tc->provides("test")->getOperation("m3"));

   double (*mmm2)(int,double) = m2;
   OperationCaller<double(int,double)> mm2("m1",mmm2,tc->engine(),caller->engine());

   double (*mmm1)(int) = m1;
   OperationCaller<double(int)> mm1("m1",mmm1,tc->engine(),caller->engine());

   double (*mmm3)(int,double,bool) = m3;
   OperationCaller<double(int,double,bool)> mm3("m1",mmm3,tc->engine(),caller->engine());
   //  OperationCaller<double(int,double,bool)> mm3("m3",tc->provides("test"));
    //  SendHandle<double(void)> h0 = m0.send();
   SendHandle<double(int)> h1 = mm1.send(1);
   SendHandle<double(int,double)> h2 = mm2.send(1,2.0);
   SendHandle<double(int,double,bool)> h3 = mm3.send(1,2.0,1);
  // h2.ready();
  if(h1.ready())
    {
      if(h1.collect(ret) == SendSuccess)
        {
          cout << ret << endl;
        }else
        {
          cout << "failed" << endl;
        }
    }

  if(h2.ready())
    {
      if(h2.collect(ret) == SendSuccess)
        {
          cout << ret << endl;
        }else
        {
          cout << "failed" << endl;
        }
    }

  if(h3.ready())
    {
      if(h3.collect(ret) == SendSuccess)
        {
          cout << ret << endl;
        }else
        {
          cout << "failed" << endl;
        }
    }
  return 1;
}

#include "os/fosi.h"
#include <stdio.h>
#include <boost/scoped_ptr.hpp>
#include <iostream>
#include "os/Thread.hpp"
#include "os/ThreadInterface.hpp"
#include "os/threads.hpp"
#include "base/ActivityInterface.hpp"
#include "Activity.hpp"
#include "internal/List.hpp"
#include "ExecutionEngine.hpp"
using namespace std;
//#include <boost/test/included/unit_test.hpp>
#define QS 10
using namespace RTT;
using namespace RTT::internal;
using namespace RTT::base;

int main()
{
  ExecutionEngine* ee = new ExecutionEngine;
  Activity m_activity(1,1,ee,"first");
  m_activity.start();
  while(1);
  return 1;
}

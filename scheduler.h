#ifndef __scheduler_h__
#define __scheduler_h__

#include <vector>
#include "task.h"
#include "user.h"
#include "virtualmachine.h"

struct Scheduler {
  static void scheduleTaskOnVM( vector<Task*> &taskL, User& owner, VM* (*scheduleVM2Task)(User&, Task&) );
};

#endif

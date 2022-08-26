#ifndef __vmschedule_h__
#define __vmschedule_h__

#include <vector>
#include "user.h"
#include "node.h"
#include "virtualmachine.h"
#include "task.h"

struct VMSchedule {
  static VM* selectVMNaive( User& owner, Task& task );
  static VM* selectVMByNbTasks( User& owner, Task& task );
  static VM* selectVMByNbInstructions( User& owner, Task& task );
};

#endif

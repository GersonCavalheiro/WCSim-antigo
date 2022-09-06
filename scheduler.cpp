#include <vector>
#include "scheduler.h"
#include "task.h"
#include "user.h"
#include "virtualmachine.h"

void Scheduler::scheduleTaskOnVM( vector<Task*> &taskL, User& owner, VM* (*scheduleVM2Task)(User&, Task&) ) {
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it ) {
    if( (*it)->getStatus() != waiting_t ) exit(0);
    if( (*it)->getStatus() == running_t ) exit(0);
    auto vm = scheduleVM2Task(owner,*(*it));
    vm->pushTask(*it);
    (*it)->setVMRunning(vm);
  }
}

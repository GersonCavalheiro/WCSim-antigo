#include <vector>
#include "vmschedule.h"
#include "user.h"
#include "node.h"
#include "virtualmachine.h"
#include "task.h"

VM* VMSchedule::selectVMNaive( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMPool();
  return *vmL.begin();
}

VM* VMSchedule::selectVMByNbTasks( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMPool();
  auto it = owner.getVMPool().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMPool().end() ; ++it )
    if( vm->getLoadNbTasks() > (*it)->getLoadNbTasks() )
      vm = *it;

  return vm;
}

VM* VMSchedule::selectVMByNbInstructions( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMPool();
  auto it = owner.getVMPool().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMPool().end() ; ++it )
    if( vm->getLoadNbInstructions() > (*it)->getLoadNbInstructions() )
      vm = *it;

  return vm;
}


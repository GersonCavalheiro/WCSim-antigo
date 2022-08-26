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
    if( vm->loadNbTasks() > (*it)->loadNbTasks() )
      vm = *it;

  return vm;
}

VM* VMSchedule::selectVMByNbInstructions( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMPool();
  auto it = owner.getVMPool().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMPool().end() ; ++it )
    if( vm->loadNbInstructions() > (*it)->loadNbInstructions() )
      vm = *it;

  return vm;
}


#include <stdlib.h>
#include <vector>
#include "selectionsch.h"
#include "user.h"
#include "node.h"
#include "host.h"
#include "virtualmachine.h"
#include "task.h"

VM* VMSelection::random( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMPool();
  return vmL[rand()%vmL.size()];
  return *vmL.begin();
}

VM* VMSelection::load( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMPool();
  auto it = owner.getVMPool().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMPool().end() ; ++it )
    if( vm->getLoadNbTasks() > (*it)->getLoadNbTasks() )
      vm = *it;

  return vm;
}

VM* VMSelection::rate( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMPool();
  auto it = owner.getVMPool().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMPool().end() ; ++it )
    if( vm->getVMips() > (*it)->getVMips() )
      vm = *it;

  return vm;
}

VM* VMSelection::selectVMByNbInstructions( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMPool();
  auto it = owner.getVMPool().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMPool().end() ; ++it )
    if( vm->getLoadNbInstructions() > (*it)->getLoadNbInstructions() )
      vm = *it;

  return vm;
}

Host* random( User& owner, Instance& vm ) {
  vector<Host*>& hostsL = owner.getNode()->getHostsList();
  return hostsL[rand()%hostsL.size()];
}

Host* bestFit( User& owner, Instance& vm );
Host* worstFit( User& owner, Instance& vm );


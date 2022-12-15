#include <stdlib.h>
#include <vector>
#include "selectionsch.h"
#include "user.h"
#include "node.h"
#include "host.h"
#include "virtualmachine.h"
#include "task.h"

VM* VMSelection::random( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMList();
  return vmL[rand()%vmL.size()];
  return *vmL.begin();
}

VM* VMSelection::circular( User& owner, Task& task ) {
  static int p = 0;
  VM *choice = owner.getVMList()[p];

  ++p;
  p = (p == owner.getVMList().size()) ? 0 : p;

  return choice;
}

VM* VMSelection::load( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMList();
  auto it = owner.getVMList().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMList().end() ; ++it )
    if( vm->getLoadNbTasks() > (*it)->getLoadNbTasks() )
      vm = *it;

  return vm;
}

VM* VMSelection::rate( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMList();
  auto it = owner.getVMList().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMList().end() ; ++it )
    if( vm->getVMips() > (*it)->getVMips() )
      vm = *it;

  return vm;
}

VM* VMSelection::selectVMByNbInstructions( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMList();
  auto it = owner.getVMList().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMList().end() ; ++it )
    if( vm->getLoadNbInstructions() > (*it)->getLoadNbInstructions() )
      vm = *it;

  return vm;
}

Host* HostSelection::random( User& owner, Instance& vm ) {
  vector<Host*>& hostsL = owner.getNode()->getHostsList();
  return hostsL[rand()%hostsL.size()];
}

Host* HostSelection::circular( User& owner, Instance& vm ) {
  static int p = 0;
  Host *choice = owner.getNode()->getHostsList()[p];

  ++p;
  p = (p == owner.getNode()->getHostsList().size()) ? 0 : p;
  
  return choice;
}

Host* HostSelection::rate( User& owner, Instance& vm ) {
  vector<Host*>& hostsL = owner.getNode()->getHostsList();
  Host  *choice = *hostsL.begin();
  float  cRate   = choice->getRate();

  cout << "Rate [" << choice->getId() << "] = " << choice->getRate() << endl;

  for( auto it = hostsL.begin() + 1 ; it != hostsL.end() ; ++it ) {
    cout << "Rate [" << (*it)->getId() << "] = " << (*it)->getRate() << endl;
    if( (*it)->getRate() > cRate ) {
      choice = *it;
      cRate   = choice->getRate();
    }
  }
  
  return choice;
}

Host* HostSelection::bestFit( User& owner, Instance& vm ) { return (Host*) NULL; }
Host* HostSelection::worstFit( User& owner, Instance& vm ) { return (Host*) NULL; }


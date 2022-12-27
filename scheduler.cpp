#include <stdlib.h>
#include <vector>

#include "loadevaluator.h"
#include "scheduler.h"
#include "user.h"
#include "node.h"
#include "host.h"
#include "virtualmachine.h"
#include "task.h"

void Scheduler::nodeBalancer() {
  vector<BareMetal*> *hosts = LoadEvaluator::overLoaded();
  for( auto it = hosts->begin() ; it != hosts->end() ; ++it )
    if( (*it)->getUtilizationRate() <= 0.5 ) {
     cout << "Migrar: " << (*it)->getHostName() << endl;     
     auto vm = ((*it)->getVMMap().begin())->second;
     auto receiver = Scheduler::receiverNodeSelection(*(vm->getRunningHost()));
     vm->migrate(receiver);
     new InstanceResumeEv(vm,GlobalClock::get()+1);
    }
}

void Scheduler::cloudBalancer() {
  vector<BareMetal*> *hosts = LoadEvaluator::overLoaded();
  for( auto it = hosts->begin() ; it != hosts->end() ; ++it )
    if( (*it)->getUtilizationRate() <= 0.25 ) {
     cout << "Migrar: " << (*it)->getHostName() << endl;     
     auto vm = ((*it)->getVMMap().begin())->second;
     auto receiver = Scheduler::receiverCloudSelection(*(vm->getRunningHost()));
     vm->migrate(receiver);
     new InstanceResumeEv(vm,GlobalClock::get()+10);
    }
}

void Scheduler::cloudBursting() {
}

VM *Scheduler::vmSelection( User& owner, Task& task ) {
  return VMSelection::random(owner,task);
  return VMSelection::circular(owner,task);
  return VMSelection::fixed(owner,task);
}

Host *Scheduler::hostSelection( Node& node, Instance& vm ) {
 return HostSelection::random(node,vm);
 return HostSelection::circular(node,vm);
 return HostSelection::fixed(node,vm);
}

Host *Scheduler::receiverNodeSelection( Host& sender ) {
  return VMMigrationHostSelection::randomNodeReceiver(sender);
}

Host *Scheduler::receiverCloudSelection( Host& sender ) {
  abort();
  return VMMigrationHostSelection::randomNodeReceiver(sender);
}

// ------------------------

VM* VMSelection::fixed( User& owner, Task& task ) {
  return *(owner.getVMList()).begin();
  /*
  vector<VM*>& vmL = owner.getVMList();
  return *vmL.begin();*/
}

VM* VMSelection::random( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMList();
  return vmL[rand()%vmL.size()];
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

Host* HostSelection::fixed( Node& node, Instance& vm ) {
  return *(node.getHostsList().begin());
}

Host* HostSelection::random( Node& node, Instance& vm ) {
  vector<Host*>& hostsL = node.getHostsList();
  return hostsL[rand()%hostsL.size()];
}

Host* HostSelection::circular( Node& node, Instance& vm ) {
  static int p = 0;
  Host *choice = node.getHostsList()[p];

  ++p;
  p = (p == node.getHostsList().size()) ? 0 : p;
  
  return choice;
}

Host* HostSelection::rate( Node& node, Instance& vm ) {
  vector<Host*>& hostsL = node.getHostsList();
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

Host* HostSelection::bestFit( Node& node, Instance& vm ) { return (Host*) NULL; }
Host* HostSelection::worstFit( Node& node, Instance& vm ) { return (Host*) NULL; }

Host* VMMigrationHostSelection::randomNodeReceiver( Host& source ) { 
  int n = rand()%(source.getNode()->getHostsList().size());
  Host *choice = source.getNode()->getHostsList()[n];
  if( choice >= &source ) choice = source.getNode()->getHostsList()[n];
  return (Host*) choice;
}

Host* VMMigrationHostSelection::randomCloudReceiver( Host& source ) { 
  int n = rand()%(source.getNode().getNodesList().size()-1);
  if( source.getNode()->getId() == n ) ++n;
  int h = rand()%source.getNode()->getNodesList().size();
  Host *choice = source.getNode()->getNodesList()[n].getHostsList()[n];
  return (Host*) choice;
}

Host* VMMigrationHostSelection::circularNodeReceiver( Host& source ) {
  return (Host*) NULL;
}

Host* VMMigrationHostSelection::randomGlobalReceiver( Host& source ) { 
  int n = rand()%(Host::getNbHosts()-1);
  if( n >= source.getId() ) ++n;
  return (Host*) Host::getHostPtrById(n);
}

Host* VMMigrationHostSelection::circularGlobalReceiver( Host& source ) { 
  static int n = 0;
  if( n == source.getId() ) ++n;
  if( n >= Host::getNbHosts() ) n = 0;
  return (Host*) Host::getHostPtrById(n++);
}

VM* VMMigrationVMSelection::random( Host& sender ) {
  if( sender.getVMMap().size() <= 1 ) return (VM*)NULL;
  auto it = sender.getVMMap().begin();
  std::advance(it,rand()%sender.getVMMap().size());
  return (VM*) it->second;
}


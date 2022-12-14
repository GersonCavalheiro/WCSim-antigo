#include <iostream>
#include <vector>
#include <string.h>

#include "virtualmachine.h"
#include "host.h"
#include "node.h"
#include "task.h"
#include "user.h"
#include "instance.h"

using namespace std;

vector<VM*> VM::vmL;

VM::VM(Node *node, User* owner ) :
#ifdef FATVM
    FatInstance(node, owner) {
#elif THINVM
    ThinInstance(node, owner) {
#else
    Instance(node, owner) {
#endif
  componentName = strdup(__func__);
  vmL.push_back(this);
}

void VM::suspend() {
  this->localSchedule();
  this->setStatus(suspended);
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
    if( (*it)->getStatus() == running_t )
      (*it)->setStatus(blocked_t);

  getRunningHost()->releaseCore( (taskL.size() >= getVCores())
		                 ? getVCores()
		                 : taskL.size() );
}

void VM::resume() {
  this->localSchedule();
  getRunningHost()->pinCore( (taskL.size() >= getVCores())
		             ? getVCores()
		             : taskL.size() );
  this->setStatus(alive);
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it ) {
    if( (*it)->getStatus() == blocked_t ) {
      (*it)->setStatus(running_t);
      (*it)->setDataStamp();
    }
  }
}

void VM::migrate( int hostId ) {
  setRunningHost(Host::getHostPtrById(hostId));
}


void VM::pushTask( Task *task ) {
  //cout << "PushTask : " << GlobalClock::get() << endl;
  this->place(task);
  task->setStatus(running_t);
// ---- Setar fim da tarefa e atualizar fim de todas
  task->setDataStamp();
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
    if( *it == task ) { cout << "existe\n"; exit(0); }
  taskL.push_back(task);
  this->localSchedule();
  ++runningTasks;
}

void VM::popTask( Task *task ) {
  //cout << "PopTask : " << GlobalClock::get() << endl;
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
    if( *it == task ) {
      --runningTasks;
      task->setStatus(completed_t);
      this->unplace(task);
      taskL.erase(it);
      break;
    }
  this->localSchedule();
}

void VM::localSchedule() {
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it ) {
    if( (*it)->getStatus() == running_t ) {
      this->avanceTask(*it);
      (*it)->setDataStamp();
      if( (*it)->getRemainingInstructions() <= 0 )
        new TaskFinishEv( *it, GlobalClock::get() );
    }
  }
}

int VM::getLoadNbInstructions() {
  int nbInstructions = 0;
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
    if( (*it)->getStatus() == running_t )
      nbInstructions += (*it)->getNbInstructions();
  return nbInstructions; 
}

VM* VM::createNewVM(User* owner) {
  return new VM(owner->getNode(),owner);
}

vector<VM*>* VM::createVMPool( int nbVMs, Host *host, User *owner ) {
  vector<VM*>* vmPool = new vector<VM*>();

  for( ; nbVMs > 0 ; --nbVMs ) vmPool->push_back(new VM(owner->getNode(),owner));

  return vmPool;
}

void VM::printAllVMs() {
  //cout << "VM size: " << vmL.size() << endl;
  for( auto it = vmL.begin() ; it != vmL.end() ; ++it )
    cout << *(*it) << endl;
}

ostream& operator<<( ostream& out, vector<VM>& vms ) {
  vector<VM>::iterator it = vms.begin();
  for( ; it != vms.end() ; ++it ) out << *it << endl;
  return out;
}

ostream& operator<<( ostream& out, VM& vm ) {
  out << "VM(" << vm.getId() << ")[" << vm.getOwner()->getId()
      << "," << vm.getSourceNode()->getId() << "/"
      << vm.getRunningHost()->getId() << "]";
  return out;
}


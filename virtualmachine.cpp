#include <iostream>
#include <vector>
#include "virtualmachine.h"
#include "node.h"
#include "task.h"
#include "user.h"
#include "instance.h"

using namespace std;

vector<VM*> VM::vmL;
int VM::vmIdCounter = 0;

VM::VM( Node* node, User* owner ) :
#ifdef FATVM
    FatInstance(node), owner(owner) {
#elif THINVM
    ThinInstance(node), owner(owner) {
#else
    Instance(node), owner(owner) {
#endif
  vmId = vmIdCounter++;
  vmL.push_back(this);
}

void VM::suspend() {
  this->localSchedule();
  this->setStatus(suspended);
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
    if( (*it)->getStatus() == running_t )
      (*it)->setStatus(blocked_t);
}

void VM::resume() {
  this->setStatus(alive);
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it ) {
    if( (*it)->getStatus() == blocked_t ) {
      (*it)->setStatus(running_t);
      (*it)->setDataStamp();
    }
  }
}

void VM::migrate( int nodeId ) {
  setRunningNode(Node::getNodePtrById(nodeId));
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

VM* VM::createNewVM( Node* node, User* owner ) {
  return new VM(node,owner);
}

vector<VM*>* VM::createVMPool( int nbVMs, Node* node, User* owner ) {
  vector<VM*>* vmPool = new vector<VM*>();

  for( ; nbVMs > 0 ; --nbVMs ) vmPool->push_back(new VM(node,owner));

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

ostream& operator<<( ostream& out, const VM& vm ) {
  out << "VM(" << vm.vmId << ")[" << vm.getOwner()->getId() << ","
      << vm.getSourceNode()->getId() << "/" << vm.getRunningNode()->getId() << "]";
  return out;
}


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
    Instance(node), owner(owner) {
  vmId = vmIdCounter++;
  vmL.push_back(this);
}

void VM::pushTask( Task *task ) {
  this->place(task);
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
    if( *it == task ) { cout << "existe\n"; exit(0); }
// ---- Setar fim da tarefa e atualizar fim de todas
  task->setDataStamp();
  taskL.push_back(task);
  if( taskL.size() <= this->getVCores() ) {
   new TaskFinishEv( task, GlobalClock::get()+(float)task->getNbInstructions()/this->getVMips() );
  } else {
    // for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
    //   ;;;
   new TaskFinishEv( task,
		     (int) GlobalClock::get()+
		     ((float)task->getNbInstructions()/
		     ((float)this->getVCores()*this->getVMips()/
		     taskL.size())) );
  }
    
// ----
  ++runningTasks;
}

void VM::popTask( Task *task ) {
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
    if( *it == task ) {
      this->unplace(task);
      taskL.erase(it);
      --runningTasks;
      return;
    }
}

//int VM::getNodeRunningId() const  { return running->getId(); }


int VM::getLoadNbInstructions() {
  int nbInstructions = 0;
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
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
  cout << "VM size: " << vmL.size() << endl;
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


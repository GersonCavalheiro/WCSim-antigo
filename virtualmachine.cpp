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

void VM::pushTask( Task *task ) {
  cout << "PushTask : " << GlobalClock::get() << endl;
  this->place(task);
  task->setStatus(running_t);
// ---- Setar fim da tarefa e atualizar fim de todas
  task->setDataStamp();
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
    if( *it == task ) { cout << "existe\n"; exit(0); }
  taskL.push_back(task);
  this->avanceInstructions();
  ++runningTasks;
}

void VM::popTask( Task *task ) {
  cout << "PopTask : " << GlobalClock::get() << endl;
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
    if( *it == task ) {
      --runningTasks;
      task->setStatus(completed_t);
      this->unplace(task);
      taskL.erase(it);
      break;
    }
  this->avanceInstructions();
}

void VM::avanceInstructions() {
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it ) {
    (*it)->avanceInstructions((GlobalClock::get()-(*it)->getDataStamp())                               *getObservedMips());
    (*it)->setDataStamp();
    if( (*it)->getRemainingInstructions() <= 0 ) {
      new TaskFinishEv( *it, GlobalClock::get() );
    }
  }
}

int VM::getLoadNbInstructions() {
  int nbInstructions = 0;
  for( auto it = taskL.begin() ; it != taskL.end() ; ++it )
    nbInstructions += (*it)->getNbInstructions();
  return nbInstructions; 
}

void VM::setDeliveredSpeed( int bareMetalSpeed ) {
  // Primeiro simula a execucao das tarefas considerando
  // o ultimo speed delivered
  this->avanceInstructions();
  // Depois atualiza o speed para o proximo passo
  cout << "Task.Size = " << taskL.size() << ", VCores = " << getVCores() << endl;
  if( taskL.size() <= getVCores() )
    setObservedMips( (getVMips() < bareMetalSpeed) ? getVMips()
		                                   : bareMetalSpeed );
  else setObservedMips(((float)(taskL.size()*bareMetalSpeed)/getVCores()));
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


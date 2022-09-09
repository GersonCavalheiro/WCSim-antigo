#include "instance.h"
#include "node.h"
#include "task.h"

Instance::Instance( Node *n, int vCores, int vMips, int vRam)
          : source(n), vCores(vCores), vMips(vMips), vRam(vRam),
     running(n), nbTasks(0) { }

void Instance::goHome() {
  this->migrate(source->getId());
}

void Instance::place( Task *t ) {
  occupedVRam += t->getRam();
  if( nbTasks < vCores ) running->pinCore();
  ++nbTasks;
}

void Instance::avanceTask( Task *t ) {
  auto actualMips = (running->getActualMips() < this->getVMips())
                    ? running->getActualMips()
                    : this->getVMips() * running->utilizationRate();
  t->avanceInstructions((GlobalClock::get() - t->getDataStamp())
                        * actualMips);
/*
  cout << "Avance(" << getName() << "): "
       << "bmMips = " <<  running->getMips()
       << ", vmActualMips = " << actualMips
       << ", bmActualMips = " <<  running->getActualMips()
       << endl;
*/
}

void Instance::unplace( Task *t ) {
  occupedVRam -= t->getRam();
  if( nbTasks <= vCores ) running->releaseCore();
  --nbTasks;
}

bool Instance::fitRam( Task *t ) {
  return (vRam-occupedVRam) >= t->getRam();
}

int Instance::getRunningNodeId() {
  return running->getId();
}

/*
void ThinInstance::avanceTask( Task *t ) {
  auto actualMips = 10;
  t->avanceInstructions((GlobalClock::get() - t->getDataStamp())
                        * actualMips);
}

void FatInstance::avanceTask( Task *t ) {
  auto actualMips = 1000;
  t->avanceInstructions((GlobalClock::get() - t->getDataStamp())
                        * actualMips);
}
*/



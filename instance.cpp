#include "instance.h"
#include "node.h"
#include "task.h"

void Instance::place( Task *t ) {
  occupedVRam += t->getRam();
  if( nbTasks < vCores ) running->pinCore();
  ++nbTasks;
  observedMips = running->getObservedSpeed();
}

void Instance::unplace( Task *t ) {
  occupedVRam -= t->getRam();
  if( nbTasks <= vCores ) running->releaseCore();
  --nbTasks;
  observedMips = running->getObservedSpeed();
}

bool Instance::fitRam( Task *t ) { return (vRam-occupedVRam) >= t->getRam(); }

int Instance::getRunningNodeId() { return running->getId(); }


#include "instance.h"
#include "node.h"
#include "task.h"

Instance::Instance( Node *n, int vCores, int vMips, int vRam)
          : source(n), vCores(vCores), vMips(vMips), vRam(vRam),
     running(n), nbTasks(0) {
  observedMips = (vMips <= n->getObservedSpeed() ) ? vMips
	         : n->getObservedSpeed();
}

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


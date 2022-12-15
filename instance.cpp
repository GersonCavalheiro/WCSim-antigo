#include <vector>

#include "instance.h"
#include "host.h"
#include "node.h"
#include "selectionsch.h"
#include "task.h"
#include "user.h"

vector<Instance*> Instance::instanceL;
int Instance::nbInstances = 0;

// SCHEDULE
//         VM PLACEMENT SELECTION
Instance::Instance( Node *n, User *u, int vCores, int vMips, int vRam)
          : sourceNode(n), owner(u), vCores(vCores), vMips(vMips),
	    vRam(vRam), nbTasks(0) {
  id = nbInstances++;
  instanceL.push_back(this);
  //running = HostSelection::circular(*(owner->getNode()),*this);
  running = SchedulePolice::hostSelection(*(owner->getNode()),*this);
  running->pushVM(this);
}

void Instance::goHome() {
  this->migrate(sourceNode->getId());
}

void Instance::place( Task *t ) {
  occupedVRam += t->getRam();
  if( nbTasks < vCores ) running->pinCore();
  ++nbTasks;
}

void Instance::unplace( Task *t ) {
  occupedVRam -= t->getRam();
  if( nbTasks <= vCores ) running->releaseCore();
  --nbTasks;
}

void Instance::avanceTask( Task *t ) {
  auto actualMips = (running->getActualMips() < this->getVMips())
                    ? running->getActualMips()
                    : this->getVMips() * running->utilizationRate();
  int executed = actualMips * (GlobalClock::get() - t->getDataStamp());
  t->hup( executed );
  this->getOwner()->billing(running->getId(),executed);
}

bool Instance::fitRam( Task *t ) {
  return (vRam-occupedVRam) >= t->getRam();
}

int Instance::getRunningHostId() {
  return running->getId();
}

/*
void ThinInstance::avanceTask( Task *t ) {
  auto actualMips = 10;
  t->hup((GlobalClock::get() - t->getDataStamp())
                        * actualMips);
}

void FatInstance::avanceTask( Task *t ) {
  auto actualMips = 1000;
  t->hup((GlobalClock::get() - t->getDataStamp())
                        * actualMips);
}
*/



#include <iostream>
#include <list>
#include "simulator.h"
#include "node.h"
#include "cloud.h"
#include "event.h"
#include "globalclock.h"

using namespace std;

int          Simulator::eventCount = 0;
list<Event*> Simulator::eventL;

void Simulator::printAllEvents() {
  int i = 0;
  for( auto it = eventL.begin() ; it != eventL.end() ; ++it, ++i )
    cout << "[" << i << "]" << *(*it) << endl;
}

void Simulator::run() {
  Event *nEv;
  int ant;

  GlobalClock::set(eventL.front()->getDate());

  //new InstanceSuspendEv(*(Instance::getInstancesL().begin()),1000);
  //new InstanceResumeEv(*(Instance::getInstancesL().begin()),3000);
  new MigrationStartEv(*(Node::getNodeList().begin()),1000);

  while( !eventL.empty() || Cloud::uncompletedTasks() > 0 ) {
    if( !eventL.empty() && eventL.front()->getDate() == GlobalClock::get() ) {
      Event* ev = eventL.front();
      eventL.pop_front();
      //cout << *ev << endl;
      ev->execute();
      delete(ev);
    }
    int soma = 0;
    auto it = Instance::getInstancesL().begin();
    for( ; it != Instance::getInstancesL().end() ; ++it ) {
      (*it)->localSchedule();
      soma += (*it)->getLoadNbTasks();
    }
    if( !eventL.empty() && eventL.front()->getDate() != GlobalClock::get() )
      GlobalClock::set(GlobalClock::get()+1);
  }

  cout << "Uncompleted tasks: " << Cloud::uncompletedTasks() << endl;
}

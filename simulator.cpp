#include <iostream>
#include <list>

#include "simulator.h"
#include "schedulepolice.h"
#include "host.h"
#include "node.h"
#include "cloud.h"
#include "event.h"
#include "globalclock.h"

using namespace std;

int          Simulator::eventCount = 0;
list<Event*> Simulator::eventL;
int          Simulator::lastBoTFinishDate = 0; //Updated by BoTFinishEv::execute
int          Simulator::lastTaskFinishDate = 0; //Updated by TaskFinishEv::execute
void Simulator::printAllEvents() {
  int i = 0;
  for( auto it = eventL.begin() ; it != eventL.end() ; ++it, ++i )
    cout << "[" << i << "]" << *(*it) << endl;
}

void Simulator::run() {
  Event *nEv;
  int ant;

  GlobalClock::set(eventL.front()->getDate());

  new SenderInitiatedMigrationEv(Host::getHostPtrById(0),1000);
  new SenderInitiatedMigrationEv(Host::getHostPtrById(1),2000);
  new SenderInitiatedMigrationEv(Host::getHostPtrById(2),3000);
  new SenderInitiatedMigrationEv(Host::getHostPtrById(3),4000);
  new SenderInitiatedMigrationEv(Host::getHostPtrById(4),5000);
  new SenderInitiatedMigrationEv(Host::getHostPtrById(5),6000);

  /*
  new SenderInitiatedMigrationEv(Host::getHostPtrById(0),10000);
  new SenderInitiatedMigrationEv(Host::getHostPtrById(1),20000);
  new SenderInitiatedMigrationEv(Host::getHostPtrById(2),30000);
  new SenderInitiatedMigrationEv(Host::getHostPtrById(3),40000);
  new SenderInitiatedMigrationEv(Host::getHostPtrById(4),50000);
  new SenderInitiatedMigrationEv(Host::getHostPtrById(5),60000);
*/

  while( !eventL.empty() || Cloud::uncompletedTasks() > 0 ) {
    if( !eventL.empty() ) {
      Event* ev = eventL.front();
      if( ev->getDate() == GlobalClock::get() ) {
        eventL.pop_front();
        // cout << *ev << endl;
        ev->execute();
       delete(ev);
      } else GlobalClock::set(GlobalClock::get()+1);
    }
    SchedulePolice::localSchedule();
  }

  cout << "Uncompleted tasks: " << Cloud::uncompletedTasks() << endl;
}

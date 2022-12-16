#include <iostream>
#include <list>

#include "simulator.h"
#include "schedulepolice.h"
#include "node.h"
#include "cloud.h"
#include "event.h"
#include "globalclock.h"

using namespace std;

int          Simulator::eventCount = 0;
list<Event*> Simulator::eventL;
int          Simulator::lastBoTFinishDate = 0; //Updated by BoTFinishEv::execute
void Simulator::printAllEvents() {
  int i = 0;
  for( auto it = eventL.begin() ; it != eventL.end() ; ++it, ++i )
    cout << "[" << i << "]" << *(*it) << endl;
}

void Simulator::run() {
  Event *nEv;
  int ant;

  GlobalClock::set(eventL.front()->getDate());

  new MigrationStartEv(*(Node::getNodeList().begin()),1000);

  while( !eventL.empty() || Cloud::uncompletedTasks() > 0 ) {
    if( !eventL.empty() ) {
      Event* ev = eventL.front();
      eventL.pop_front();
      GlobalClock::set(ev->getDate());
      // cout << *ev << endl;
      ev->execute();
      delete(ev);
    } else // in this case: Cloud::uncompletedTasks() is > 0
      GlobalClock::set(GlobalClock::get()+1);

    SchedulePolice::localSchedule();
  }

  cout << "Uncompleted tasks: " << Cloud::uncompletedTasks() << endl;
}

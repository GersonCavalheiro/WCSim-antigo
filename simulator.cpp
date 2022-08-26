#include <iostream>
#include <list>
#include "simulator.h"
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

  while( !eventL.empty() ) {
    Event* ev = eventL.front();
    eventL.pop_front();
    GlobalClock::set(ev->getDate());
    ev->execute();
    delete(ev);
  }
}

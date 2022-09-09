#include <iostream>
#include <list>
#include "simulator.h"
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

/* run() {
  Event *nEv;
  int ant;

  while( !eventL.empty() ) {
    Event* ev = eventL.front();
    eventL.pop_front();
    GlobalClock::set(ev->getDate());
    ev->execute();
    delete(ev);

    if( Cloud::uncompletedTasks() > 0 ) {
      auto vmL = VM::getVmList();
      for( auto it = vmL->begin() ; it != vmL->end() ; ++it )
	(*it)->avanceInstructions();
      }

    cout << "Uncompleted tasks: " << Cloud::uncompletedTasks() << endl;
   }

   cout << "Finaleira\n";
   while( Cloud::uncompletedTasks() > 0 ) {
      cout << "uncompletedTasks: " << Cloud::uncompletedTasks() << endl;
     GlobalClock::set(GlobalClock::get()+1);
     auto vmL = VM::getVmList();
     for( auto it = vmL->begin() ; it != vmL->end() ; ++it )
       (*it)->avanceInstructions();
     while( !eventL.empty() ) { 
       Event* ev = eventL.front();
       eventL.pop_front();
       GlobalClock::set(ev->getDate());
       ev->execute();
       delete(ev);
     }
   } 

   cout << "Uncompleted tasks: " << Cloud::uncompletedTasks() << endl;
}
*/
void Simulator::run() {
  Event *nEv;
  int ant;

  GlobalClock::set(eventL.front()->getDate());

  while( !eventL.empty() || Cloud::uncompletedTasks() > 0 ) {
    if( !eventL.empty() && eventL.front()->getDate() == GlobalClock::get() ) {
      Event* ev = eventL.front();
      eventL.pop_front();
      ev->execute();
      delete(ev);
    }
    int soma = 0;
    auto vmL = VM::getVmList();
    for( auto it = vmL->begin() ; it != vmL->end() ; ++it ) {
      (*it)->localSchedule();
      soma += (*it)->getLoadNbTasks();
    }
    if( !eventL.empty() && eventL.front()->getDate() != GlobalClock::get() )
      GlobalClock::set(GlobalClock::get()+1);
//    cout << "[" << GlobalClock::get() << "] Placed tasks: " << soma << " , "
//         << Cloud::uncompletedTasks() << endl;
  }

  cout << "Uncompleted tasks: " << Cloud::uncompletedTasks() << endl;
}

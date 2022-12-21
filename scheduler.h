#ifndef __scheduler_h_
#define __scheduler_h_

#include <vector>
#include "user.h"
#include "instance.h"
#include "host.h"
#include "virtualmachine.h"
#include "task.h"

struct Scheduler {
  static inline void localSchedule() {
    auto it = Instance::getInstancesL().begin();
    for( ; it != Instance::getInstancesL().end() ; ++it )
      if( (*it)->getStatus() == alive )
         (*it)->localSchedule();
  }
  static VM*   vmSelection( User& owner, Task& task );
  static Host* hostSelection( Node& node, Instance& vm );
  static Host* receiverSelection( Host& sender );
};

struct VMSelection {
  static VM* random( User& owner, Task& task );
  static VM* circular( User& owner, Task& task );
  static VM* load( User& owner, Task& task );
  static VM* rate( User& owner, Task& task );
  static VM* selectVMByNbInstructions( User& owner, Task& task );
};

struct HostSelection {
  static Host* random( Node& node, Instance& vm );
  static Host* circular( Node& node, Instance& vm );
  static Host* rate( Node& node, Instance& vm );
  static Host* bestFit( Node& node, Instance& vm );
  static Host* worstFit( Node& node, Instance& vm );
};

struct VMMigration {
  static Host* randomReceiver( Host& sender );
  static Host* circularReceiver( Host& sender );
};

#endif

#ifndef __selectionsch_h__
#define __selectionsch_h__

#include <vector>
#include "user.h"
#include "host.h"
#include "virtualmachine.h"
#include "task.h"

struct SchedulePolice {
  static VM*   vmSelection( User& owner, Task& task );
  static Host* hostSelection( Node& node, Instance& vm );
  static Host* vmMigration( User& owner, Instance& vm );
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
  static Host* random( User& owner, Instance& vm );
  static Host* circular( User& owner, Instance& vm );
};

#endif

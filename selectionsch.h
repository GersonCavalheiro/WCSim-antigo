#ifndef __selectionsch_h__
#define __selectionsch_h__

#include <vector>
#include "user.h"
#include "host.h"
#include "virtualmachine.h"
#include "task.h"

struct VMSelection {
  static VM* random( User& owner, Task& task );
  static VM* load( User& owner, Task& task );
  static VM* rate( User& owner, Task& task );
  static VM* selectVMByNbInstructions( User& owner, Task& task );
};

struct HostSelection {
  static Host* random( User& owner, Instance& vm );
  static Host* bestFit( User& owner, Instance& vm );
  static Host* worstFit( User& owner, Instance& vm );
};

#endif

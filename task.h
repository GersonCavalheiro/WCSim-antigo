#ifndef __task_h__
#define __task_h__

#include <stdlib.h>
#include <vector>

#include "utils.h"
#include "bot.h"
#include "node.h"
#include "virtualmachine.h"
#include "globalclock.h"
#include "component.h"

using namespace std;

class Task : public Component {
private:
  static vector<Task*> tasksL;
  static int taskCount, tasksRunning, tasksCompleted;
  vector<int> botAttr;
  STATUS st;
  int taskId, taskOwner, taskBoT, taskNode;
  BoT* myBoT;
  VM* vmRunning;
  int lastDataStamp, estimatedFinishDate;
  int miRemaining; // Amount of computation remaining

public:
  Task( BoT* myBoT, vector<int>& botAttr );

  void setStatus( int st );
  inline int getStatus() const { return st; }
  inline void  setVMRunning( VM *vm ) { vmRunning = vm; }
  inline Node* getNodeRunning() const { return vmRunning->getRunningNode(); }
  inline int   getNodeRunningId() const { return vmRunning->getRunningNodeId(); }
  inline VM*   getVMRunning() const { return vmRunning; }
  inline int   getId() const { return taskId; }
  inline int   getNbInstructions() const { return botAttr[nbInstructions]; }
  inline int   getRam() const { return botAttr[memTask]; }
  inline BoT*  getBoT() const { return myBoT; }
  inline void  setDataStamp() { lastDataStamp = GlobalClock::get(); }
  inline int   getDataStamp() const { return lastDataStamp; }
  inline int   getRemainingInstructions() const { return miRemaining; }
  inline void  avanceInstructions( int mi ) { miRemaining -= mi; }

  static inline int getNbTasks() { return taskCount; }
  static inline int getRunningTasks() { return tasksRunning; }
  static inline int getCompletedTasks() { return tasksCompleted; }
  static void removeFromTaskList( Task *t );
  friend ostream& operator<<( ostream& out, const Task& t );
};

#endif

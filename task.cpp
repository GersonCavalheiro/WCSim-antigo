#include <stdlib.h>
#include <iostream>

#include "utils.h"
#include "task.h"
#include "node.h"
#include "globalclock.h"

vector<Task*> Task::tasksL;
int           Task::taskCount = 0;

Task::Task( BoT *myBoT, vector<int>& botAttr ) : botAttr(botAttr), myBoT(myBoT) {
  if( botAttr[nDependBoT] == 0 && botAttr[arrivalBoT] <= GlobalClock::get() ) st = ready;
  if( botAttr[nDependBoT] == 0 && botAttr[arrivalBoT] > GlobalClock::get() )  st = waiting;
  taskId = taskCount++;
  st = waiting;
  taskOwner = botAttr[ownerId];
  taskNode = botAttr[nodeId];
  taskBoT = botAttr[botId];
  MiRemaining = botAttr[nbInstructions];
  lastDataStamp = GlobalClock::get();
  tasksL.push_back(this);
}

//int Task::getNodeRunningId() const { return myVM.getNodeRunning->getId(); }

void Task::setStatus( int st ) {
  if( this->st == st )
    if( st == running ) cout << "Running!!!!!!!!!!!!!!!!!!!!!!!!: " << getId() << endl;
    else if( st == completed ) cout << "Completed!!!!!!!!!!!!!!!!!!!!!!!!: " << getId() << endl;
  this->st = (STATUS) st;
  if( st == running ) myBoT->aTaskRunning();
  if( st == completed ) myBoT->aTaskCompleted();
}

ostream& operator<<( ostream& out, const Task& t ) {
  out << "(t" << t.taskId << ":" << t.taskBoT << "," << t.taskOwner
      << "," << t.taskNode << ":" << t.botAttr[arrivalBoT] << ")";
  return out;
}


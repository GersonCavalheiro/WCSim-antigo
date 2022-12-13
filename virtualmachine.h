#ifndef __virtual_machine_h__
#define __virtual_machine_h__
#include <iostream> 
#include <vector>

#include "instance.h"

using namespace std;

class Host;
class User;
class Task;

#ifdef FATVM
class VM : public FatInstance {
#elif THINVM
class VM : public ThinInstance {
#else
class VM : public Instance {
#endif
private:
  static vector<VM*> vmL;
  vector<Task*> taskL;    // Task placed over this VM
  int runningTasks;

public:
  VM(Node *node, User* owner);

  static VM* createNewVM(User* owner);
  static vector<VM*> *createVMPool( int nbVMs, Host *host, User *owner );
  static void printAllVMs();
  static inline vector<VM*> *getVmList() { return &vmL; }

  inline int   getLoadNbTasks() { return taskL.size(); } 
  void localSchedule();
  int  getLoadNbInstructions(); 

  void pushTask(Task* task);
  void popTask(Task* task);

  void suspend();
  void resume();
  void migrate( int hostId );

  friend ostream& operator<<( ostream& out, VM& vm );
};

#endif

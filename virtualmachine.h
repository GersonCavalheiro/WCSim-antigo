#ifndef __virtual_machine_h__
#define __virtual_machine_h__
#include <iostream> 
#include <vector>

#include "instance.h"

using namespace std;

class Node;
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
  static int vmIdCounter; // VM id counter
  User *owner;            // VM owner
  int   vmId;             // Unique VM id
  vector<Task*> taskL;    // Task placed over this VM
  int runningTasks;

public:
  VM( Node* node, User* owner );

  static VM* createNewVM( Node* node, User* owner );
  static vector<VM*> *createVMPool( int nbVMs, Node *node, User *owner );
  static void printAllVMs();
  static inline vector<VM*> *getVmList() { return &vmL; }

  inline int   getId() const { return vmId; }
  inline User* getOwner() const { return owner; }
  inline int   getLoadNbTasks() { return taskL.size(); } 
  void avanceInstructions();
  int  getLoadNbInstructions(); 

  void setDeliveredSpeed( int bareMetalSpeed );

  void pushTask(Task* task);
  void popTask(Task* task);

  friend ostream& operator<<( ostream& out, const VM& vm );
};

#endif

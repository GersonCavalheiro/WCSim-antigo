#ifndef __virtual_machine_h__
#define __virtual_machine_h__
#include <iostream> 
#include <vector>

#include "instance.h"

using namespace std;

class Node;
class User;
class Task;

class VM {
private:
  static vector<VM*> vmL;
  static int vmIdCounter; // VM id counter
  Node *source, *running; // launched, current location
  User *owner;            // VM owner
  int   vmId;             // Unique VM id
  vector<Task*> taskL;    // Task placed over this VM
  Instance instanceOf;    // Instance class
  int runningTasks;

public:
  VM( Node* node, User* owner );
  VM( Instance inst, Node* node, User* owner );

  static VM* createNewVM( Node* node, User* owner );
  static VM* createNewVM( Instance inst, Node* node, User* owner );
  static vector<VM*>* createVMPool( int nbVMs, Node* node, User* owner );
  static vector<VM*>* createVMPool( Instance inst, int nbVMs, Node* node, User* owner );
  static void printAllVMs();

  inline int   getId() const { return vmId; }
  inline User* getOwner() const { return owner; }
  inline Node* getNodeSource() const { return source; }
  inline Node* getNodeRunning() const { return running; }
  inline int   getVCores() const  { return instanceOf.getVCores(); }
  inline int   getVMips() const  { return instanceOf.getVMips(); }
  inline int   getLoadNbTasks() { return taskL.size(); } 
  inline int   getObservedVMpis() { return (int) (taskL.size()*instanceOf.getVMips())/instanceOf.getVCores(); }
  int getNodeRunningId() const;
  int getLoadNbInstructions(); 

  inline void place(Task* task ) { instanceOf.place( task ); }
  inline void unplace(Task* task ) { instanceOf.unplace( task ); }
  void pushTask(Task* task);
  void popTask(Task* task);

  friend ostream& operator<<( ostream& out, const VM& vm );
};

#endif

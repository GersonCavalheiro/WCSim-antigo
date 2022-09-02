#ifndef __virtual_machine_h__
#define __virtual_machine_h__
#include <iostream> 
#include <vector>

#include "instance.h"

using namespace std;

class Node;
class User;
class Task;

class VM : public Instance {
private:
  static vector<VM*> vmL;
  static int vmIdCounter; // VM id counter
  //Node *source, *running; // launched, current location
  User *owner;            // VM owner
  int   vmId;             // Unique VM id
  vector<Task*> taskL;    // Task placed over this VM
  int runningTasks;

public:
  VM( Node* node, User* owner );

  static VM* createNewVM( Node* node, User* owner );
  static vector<VM*> *createVMPool( int nbVMs, Node *node, User *owner );
  static void printAllVMs();

  inline int   getId() const { return vmId; }
  inline User* getOwner() const { return owner; }
//  inline Node* getNodeSource() const { return source; }
//  inline Node* getNodeRunning() const { return running; }
  inline int   getLoadNbTasks() { return taskL.size(); } 
//  int getNodeRunningId() const;
  int getLoadNbInstructions(); 

  void pushTask(Task* task);
  void popTask(Task* task);

  friend ostream& operator<<( ostream& out, const VM& vm );
};

#endif

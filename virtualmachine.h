#ifndef __virtual_machine_h__
#define __virtual_machine_h__
#include <iostream> 
#include <vector>

using namespace std;

class Node;
class User;
class Task;

class Instance {
private:
  int vCores, vMips, vRam; // vMips, vRam per core
  int observedMips;
public:
  Instance( int vCores = 4, int vMips = 1000, int vRam = 16 ) :
          vCores(vCores), vMips(vMips), vRam(vRam) {} 
  virtual inline int getVCores() const { return vCores; }
  virtual inline int getVMips() const { return vMips; }
  virtual inline int getVRam() const { return vRam; }
};

class TinyInstance : public Instance {
public:
  TinyInstance( int vCores = 1, int vMips = 10, int vRam = 4 ) 
     : Instance( vCores, vMips, vRam ) {}
};

class FatInstance : public Instance {
public:
  FatInstance( int vCores = 4, int vMips = 1000, int vRam = 16 ) 
     : Instance( vCores, vMips, vRam ) {}
};

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
  inline int   getId() const { return vmId; }
  inline User* getOwner() const { return owner; }
  inline Node* getNodeSource() const { return source; }
  inline Node* getNodeRunning() const { return running; }
  int   getNodeRunningId() const;
  inline int   getVCores() const  { return instanceOf.getVCores(); }
  inline int   getVMips() const  { return instanceOf.getVMips(); }
  inline int   getObservedVMpis() { return (int) (taskL.size()*instanceOf.getVMips())/instanceOf.getVCores(); }

  void pushTask(Task* task);
  void popTask(Task* task);
  inline int  loadNbTasks() { return taskL.size(); } 
  int  loadNbInstructions(); 
  static VM* createNewVM( Node* node, User* owner );
  static VM* createNewVM( Instance inst, Node* node, User* owner );
  static vector<VM*>* createVMPool( int nbVMs, Node* node, User* owner );
  static vector<VM*>* createVMPool( Instance inst, int nbVMs, Node* node, User* owner );
 // bool fillVM( deque<Task> inTasks ) { }
  static void printAllVMs();
  friend ostream& operator<<( ostream& out, const VM& vm );
};

#endif

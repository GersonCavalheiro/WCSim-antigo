#ifndef __INSTANCE_H__
#define __INSTANCE_H__
#include <iostream> 
#include <vector>

class Node;
class Task;

using namespace std;

class Instance {
private:
  int vCores, vMips, vRam; // vMips, vRam per core
  int observedMips, occupedVRam;
  int nbTasks;
  Node *source, *running; // launched, current location

public:
  Instance( Node *n, int vCores = 4, int vMips = 1000, int vRam = 16 ) :
     source(n), vCores(vCores), vMips(vMips), vRam(vRam),
     running(n), nbTasks(0) {} 
  inline void setSourceNode( Node *n ) { source = n; }
  inline Node *getSourceNode() const { return source; }
  inline void setRunningNode( Node *n ) { running = n; }
  inline Node *getRunningNode() const { return running; }
  int  getRunningNodeId();
  virtual inline int  getVCores() const { return vCores; }
  virtual inline int  getNominalMips() const { return vMips; }
  virtual inline int  getVMips() const { return observedMips; }
  virtual inline int  getVRam() const { return vRam; }

  virtual void place( Task *t );
  virtual void unplace( Task *t );
  virtual bool fitRam( Task *t );
};

class ThinInstance : public Instance {
public:
  ThinInstance( Node *n, int vCores = 1, int vMips = 10, int vRam = 4 ) 
     : Instance( n, vCores, vMips, vRam ) {}
};

class FatInstance : public Instance {
public:
  FatInstance( Node *n, int vCores = 4, int vMips = 1000, int vRam = 16 ) 
     : Instance( n, vCores, vMips, vRam ) {}
};

#endif

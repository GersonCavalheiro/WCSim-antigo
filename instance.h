#ifndef __INSTANCE_H__
#define __INSTANCE_H__
#include <iostream> 
#include <vector>

class Task;

using namespace std;

class Instance {
private:
  int vCores, vMips, vRam; // vMips, vRam per core
  int observedMips, occupedVRam;
public:
  Instance( int vCores = 4, int vMips = 1000, int vRam = 16 ) :
          vCores(vCores), vMips(vMips), vRam(vRam) {} 
  virtual inline int  getVCores() const { return vCores; }
  virtual inline int  getVMips() const { return vMips; }
  virtual inline int  getVRam() const { return vRam; }
  virtual void place( Task *t );
  virtual void unplace( Task *t );
  virtual bool fitRam( Task *t );
};

class ThinInstance : public Instance {
public:
  ThinInstance( int vCores = 1, int vMips = 10, int vRam = 4 ) 
     : Instance( vCores, vMips, vRam ) {}
};

class FatInstance : public Instance {
public:
  FatInstance( int vCores = 4, int vMips = 1000, int vRam = 16 ) 
     : Instance( vCores, vMips, vRam ) {}
};

#endif

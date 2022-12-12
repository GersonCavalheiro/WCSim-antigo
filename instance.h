#ifndef __INSTANCE_H__
#define __INSTANCE_H__
#include <iostream> 
#include <vector>

#include "component.h"

class Host;
class Task;
class User;

using namespace std;

class Instance : public Component {
private:
  static int nbInstances;
  int id;
  int vCores, vMips, vRam; // vMips, vRam per core
  int occupedVRam;
  int nbTasks;
  int status;
  Host *source, *running; // launched, current location

public:
  Instance( Host *n, int vCores = 4, int vMips = 1000, int vRam = 16 );
  inline int getId() { return id; }
  inline void setSourceHost( Host *n )
                 { source = n; }
  inline Host *getSourceHost() const
                 { return source; }
  inline void setRunningHost( Host *n )
                 { running = n; }
  inline Host *getRunningHost() const
                 { return running; }
  inline void setStatus( int st )
	         { status = st; }
  inline int  getStatus()
	         { return status; }
  int  getRunningHostId();
  virtual inline int  getVCores() const
                         { return vCores; }
  virtual inline int  getVMips() const
                         { return vMips; }
  virtual inline int  getVRam() const
                         { return vRam; }
  virtual inline string getName() const
                         { return string("DefaultVM"); }
  virtual inline User* getOwner() const = 0;


  virtual void place( Task *t );
  virtual void unplace( Task *t );
  virtual bool fitRam( Task *t );
  virtual void avanceTask( Task *t );
  virtual void suspend() = 0;
  virtual void resume() = 0;
  virtual void migrate( int hostId ) = 0; 
  void goHome();
};

class ThinInstance : public Instance {
public:
  ThinInstance( Host *n, int vCores = 1, int vMips = 10, int vRam = 4 ) 
     : Instance( n, vCores, vMips, vRam ) {}
  inline string getName() const
                         { return string("ThinVM"); }
};

class FatInstance : public Instance {
public:
  FatInstance( Host *n, int vCores = 16, int vMips = 100, int vRam = 16 ) 
     : Instance( n, vCores, vMips, vRam ) {}
  inline string getName() const
                         { return string("FatVM"); }
};

#endif

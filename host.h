#ifndef __host_h__
#define __host_h__

#include <iostream>
#include <list>
#include <map>

#include "utils.h"
#include "baremetal.h"
#include "task.h"
#include "virtualmachine.h"
#include "component.h"
#include "usage.h"

class Host : public BareMetal {
  static map<int,Host*>    hostsListById;
  static map<string,Host*> hostsListByName;
  static int hostCount;
  vector<VM*> vmL;
  list<Task*> taskList; // Lista de tarefas prontas
  int nbCores;
  int id, iId, risingDate, status;
  string hostName;
  char *className;

public:
  Host( const string name, const int id, const int risingDate );
  inline int    getId()               const { return id; }
  inline int    getIId()              const { return iId; }
  inline int    getHostNb(int hostId) const
    { return hostsListById[hostId]->getIId(); }
  inline int    getRisingDate()       const { return risingDate; }
  inline int    getStatus()           const { return status; }
  inline bool   isOnline()            const { return status == online; }
  inline void   setStatus( int st )         { status = st; }
  inline string getName() const             { return hostName; }
  inline int    getNbVMs()            const { return vmL.size(); }

  void pushVM( vector<VM*>& vmPool );
  void pushVM( VM* vm );

  // void updateSpeed();

  static inline bool isActive( int hostId )
    { return hostsListById.find(hostId) != hostsListById.end(); }
  static inline int  getNbHosts()
    { return hostCount; }
  static Host* getHostPtrById( int hostId );
  static void insertHostInLists( Host *n );

  friend ostream& operator<<( ostream& out, Host& n );
  static void printAllHosts();
  void printStatus();
};

#endif


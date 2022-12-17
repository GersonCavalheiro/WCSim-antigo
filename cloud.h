#ifndef __cloud_h__
#define __cloud_h__

#include <vector>

#include "host.h"
#include "baremetal.h"
#include "user.h"
#include "component.h"


class Cloud : public Component {
  static vector<vector<int>> link;
public:
  // n: number of hosts
  // c: number of cores per host
  static Host* newHost( const string name, const int risingDate, const int bmFamilly );

  static void readCloudFile( string cloudFileName = "input/mycloud.cld");
  static void readNetworkFile( string networkFileName = "input/network.net");
  static void setLinkSpeeds( int host, string& speeds );
  static inline int getLinkSpeed( int source, int destination ) { return bandwidth(source,destination); }
  static int getLinkSpeed( int source, vector<int>& vSpeeds );
  static void deploy();
  static int  bandwidth(int source, int destination);
  static inline Host* getHostPtrById( int hostId )  { return Host::getHostPtrById( hostId ); }

  static inline int uncompletedTasks() { return Task::getNbTasks() - Task::getCompletedTasks(); }

  static void printAllLinks();
  static void printAllCloud();
  static Node *getNode(string name);
  static int delay( Instance *vm, Host *src, Host *dst );
};

#endif
 

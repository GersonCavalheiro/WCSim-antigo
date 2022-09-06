#ifndef __cloud_h__
#define __cloud_h__

#include <vector>

#include "node.h"
#include "baremetal.h"
#include "user.h"
#include "component.h"


class Cloud : public Component {
  static vector<vector<int>> link;
public:
  // n: number of nodes
  // c: number of cores per node
  static Node* newNode( const string name, const int id, const int risingDate, const int bmFamilly );

  static void readCloudFile( string cloudFileName = "input/mycloud.cld");
  static void readNetworkFile( string networkFileName = "input/network.net");
  static void setLinkSpeeds( int node, string& speeds );
  static inline int getLinkSpeed( int source, int destination ) { return bandwidth(source,destination); }
  static int getLinkSpeed( int source, vector<int>& vSpeeds );
  static void deploy();
  static int  bandwidth(int source, int destination);
  static inline Node* getNodePtrById( int nodeId )  { return Node::getNodePtrById( nodeId ); }

  static inline int uncompletedTasks() { return Task::getNbTasks() - Task::getCompletedTasks(); }

  static void printAllLinks();
  static void printAllCloud();
};

#endif
 

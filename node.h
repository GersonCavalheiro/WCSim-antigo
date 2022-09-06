#ifndef __node_h__
#define __node_h__

#include <iostream>
#include <list>
#include <map>

#include "utils.h"
#include "baremetal.h"
#include "task.h"
#include "virtualmachine.h"
#include "component.h"

class Node : public BareMetal, public Component {
  static map<int,Node*>    nodesListById;
  static map<string,Node*> nodesListByName;
  static int nodeCount;
  vector<VM*> vmL;
  list<Task*> taskList; // Lista de tarefas prontas
  int nbCores;
  int id, iId, risingDate, status;
  string nodeName;

public:
  Node( const string name, const int id, const int risingDate );
  inline int    getId()               const { return id; }
  inline int    getIId()              const { return iId; }
  inline int    getNodeNb(int nodeId) const
    { return nodesListById[nodeId]->getIId(); }
  inline int    getRisingDate()       const { return risingDate; }
  inline int    getStatus()           const { return status; }
  inline bool   isOnline()            const { return status == online; }
  inline void   setStatus( int st )         { status = st; }
  inline string getName() const             { return nodeName; }
  inline int    getNbVMs()            const { return vmL.size(); }

  void pushVM( vector<VM*>& vmPool );
  void pushVM( VM* vm );

  void updateSpeed();

  static inline bool isActive( int nodeId )
    { return nodesListById.find(nodeId) != nodesListById.end(); }
  static inline int  getNbNodes()
    { return nodeCount; }
  static Node* getNodePtrById( int nodeId );
  static void insertNodeInLists( Node *n );

  friend ostream& operator<<( ostream& out, Node& n );
  static void printAllNodes();
  void printStatus();
};

#endif


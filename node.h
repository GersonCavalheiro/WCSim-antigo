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

class Node : public Component {
  static map<int,Node*>    nodesListById;
  static map<string,Node*> nodesListByName;
  static int nodeCount;
  vector<VM*> vmL;
  list<Task*> taskList; // Lista de tarefas prontas
  int nbCores;
  int id, iId, risingDate, status;
  string nodeName;
  BareMetal *bm;

public:
  Node( const string name, const int id, const int risingDate, const int bmFamily );
  inline int    getId() const { return id; }
  inline int    getIId() const { return iId; }
  inline int    getNodeNb(int nodeId) const { return nodesListById[nodeId]->getIId(); }
  static inline int getNbNodes() { return nodeCount; }
  inline int    getSpeed() const { return bm->getSpeed(); }
  inline int    getRisingDate() const { return risingDate; }
  inline int    getStatus() const { return status; }
  inline bool   isOnline() const { return status == online; }
  inline void   setStatus( int st ) { status = st; }
  inline string getName() const { return nodeName; }
  void pushNewVM( vector<VM*>& vmPool );
  void pushNewVM( VM* vm );
  friend ostream& operator<<( ostream& out, Node& n );
  void printStatus();
  static bool isActive( int nodeId ) { return (nodesListById.find(nodeId) != nodesListById.end()) ? true : false; }
  static Node* getNodePtrById( int nodeId );
  static void insertNodeInLists( Node *n );
  static void printAllNodes();
};

#endif


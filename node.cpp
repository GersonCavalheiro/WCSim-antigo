#include <stdlib.h>
#include <iostream>
#include <list>
#include <map>

#include "node.h"
#include "utils.h"
#include "baremetal.h"
#include "task.h"

map<int,Node*>    Node::nodesListById;
map<string,Node*> Node::nodesListByName;
int Node::nodeCount = 0;

Node::Node( const string name, const int id, const int risingDate, const int bmFamily )
     : nodeName(name), id(id), risingDate(risingDate) {
  iId = nodeCount++;
  
  switch( bmFamily ) {
    case 0 : bm = new BareMetal();
             break;
    default: cout << "Error. Only Baremetal 0 is implemented\n";
             abort();
  }
  insertNodeInLists(this);
  if( risingDate > GlobalClock::get() ) {
    status = offline;
    new NodeRisingEv(this);
  }
  else status = online;
}

// Necessario pois é preciso que o nome do no ja tenha sido construido
void Node::insertNodeInLists( Node *n ) {
  nodesListById.insert(pair<int,Node*>(n->getId(),n));
  nodesListByName.insert(pair<string,Node*>(n->getName(),n));
}

ostream& operator<<( ostream& out, Node& n ) {
  out << "[" << n.iId << "|" << n.id << "|" << n.nodeName << "]";
  return out;
}

void Node::printStatus() { 
  cout << "Node: " << getName() << "/" << getId() << " has " << taskList.size()
       << " ready tasks waiting to run" << endl;
  for( list<Task*>::iterator it = taskList.begin() ; it != taskList.end() ; ++it )
    cout << *(*it) << endl;
}

Node* Node::getNodePtrById( int nodeId ) {
  auto it = nodesListById.find(nodeId);
  if( it == nodesListById.end() ) return NULL;
  return (*it).second;
}

void Node::printAllNodes() {
//  for( auto it = nodesListById.begin() ; it != nodesListById.end() ; ++it )
 //   cout << *it->second << endl;
  cout << "S = " << nodesListByName.size() << "," << nodesListById.size() << endl;
  for( auto it = nodesListByName.begin() ; it != nodesListByName.end() ; ++it )
    cout << *it->second << endl;
}

void Node::pushNewVM( vector<VM*>& vmL ) {
  for( auto it = vmL.begin() ; it != vmL.end() ; ++it )
   pushNewVM(*it);
  vmL.clear();
}

void Node::pushNewVM( VM* vm ) {
  vmL.push_back(vm); 
}


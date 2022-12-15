#include <stdlib.h>
#include <iostream>
#include <list>
#include <map>
#include <string.h>

#include "host.h"
#include "node.h"
#include "utils.h"
#include "baremetal.h"
#include "task.h"
#include "usage.h"

map<int,Host*>    Host::hostsListById;
map<string,Host*> Host::hostsListByName;
int Host::hostCount = 0;

Host::Host( const string name, const int risingDate )
     : nodeName(name), risingDate(risingDate) {
  char aux[4];
  componentName = strdup(__func__);
  id = hostCount++;
  hostName = nodeName + "H" + itoa(id,aux,10);

  cout << "Numero: " << hostName << endl;
  
  insertHostInLists(this);
  Node::pushHost(nodeName,this);
  if( risingDate > GlobalClock::get() ) {
    status = offline;
    new HostRisingEv(this);
  }
  else status = online;

  Usage::insert(getId());
}

// Necessario pois é preciso que o nome do no ja tenha sido construido
void Host::insertHostInLists( Host *n ) {
  hostsListById.insert(pair<int,Host*>(n->getId(),n));
  hostsListByName.insert(pair<string,Host*>(n->getName(),n));
}

ostream& operator<<( ostream& out, Host& n ) {
  out << "[" << n.id << "|" << n.hostName << "]";
  return out;
}

void Host::printStatus() { 
  cout << "Host: " << getName() << "/" << getId()
       << " has " << taskList.size()
       << " ready tasks waiting to run" << endl;
  for( list<Task*>::iterator it = taskList.begin() ;
       it != taskList.end() ; ++it )
    cout << *(*it) << endl;
}

Host* Host::getHostPtrById( int hostId ) {
  auto it = hostsListById.find(hostId);
  if( it == hostsListById.end() ) return NULL;
  return (*it).second;
}

void Host::printAllHosts() {
  cout << "S = " << hostsListByName.size() 
       << "," << hostsListById.size() << endl;
  for( auto it = hostsListByName.begin() ;
       it != hostsListByName.end() ; ++it )
    cout << *it->second << endl;
}

void Host::pushVM( vector<Instance*>& vmL ) {
  for( auto it = vmL.begin() ; it != vmL.end() ; ++it )
   pushVM(*it);
  vmL.clear();
}

void Host::pushVM( Instance *vm ) {
  this->place(vm);
  vmL.push_back(vm); 
}


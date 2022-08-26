#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "utils.h"
#include "user.h"
#include "task.h"
#include "cloud.h"
#include "event.h"
#include "globalclock.h"
#include "simulator.h"
#include "virtualmachine.h"
#include "vmschedule.h"

using namespace std;

int User::userCount = 0;
map<int,User*>    User::usersListById;
map<string,User*> User::usersListByName;

User::User( string name, int id, int node, int userLoginDate, int nbVMs, int vmFamily )
     : userName(name), userId(id), userNode(node), userLoginDate(userLoginDate),
       nbVMs(nbVMs), vmFamily(vmFamily) {
  ++userCount;
  usersListById.insert(pair<int,User*>(userId,this));
  usersListByName.insert(pair<string,User*>(userName,this));

  if( userLoginDate > GlobalClock::get() ) {
    status = offline;
    new UserLoginEv(this);
  }
  else {
    myHostNode = Cloud::getNodePtrById(userNode);
    if( myHostNode == NULL ) {
      cout << "User(" << userName << ") login fail: Node(" << userNode << ") is down." << endl;
      return;
    }
    rentNewVMs(nbVMs); 
    status = online;
  }
}

bool User::rentNewVMs( int n ) {
  if( n <= 0 ) return false;
  for( ; n > 0 ; --n ) {
#ifdef THINVM
    auto vm = VM::createNewVM(ThinInstance(),myHostNode,this);
#endif
#ifndef THINVM
    auto vm = VM::createNewVM(FatInstance(),myHostNode,this);
#endif
    myVMPool.push_back(vm);
  }
  return true;
}

ostream& operator<<( ostream& out, User& u ) {
  out << "(" << u.getId() << "," << u.getName() << "): " << u.myVMPool.size() << " VMs";
  return out;
}

void User::userLogin() {
  myHostNode = Cloud::getNodePtrById(userNode);
  if( myHostNode == NULL ) {
    cout << "User(" << userName << ") login fail: Node(" << userNode << ") is down." << endl;
    return;
  }
  rentNewVMs(nbVMs);
  status = online;
}

/* ------------------
 * Read password file
   ------------------ */
void User::readUserFile(string userFileName) {
  string name;
  int    userId, userNode, userLoginDate, nbVMs, vmFamily;

  std::ifstream infile(userFileName);
  cout << "User file name: " << userFileName << endl;
  
  infile >> name;
  while( !infile.eof() ) {
    infile >> userId >> userNode >> userLoginDate >> nbVMs >> vmFamily;
    if( userIdTaken(userId) || userNameTaken(name) )
      cout << "[" << __LINE__ << "] User name (" << name << ") or user id ("
           << userId << ") already taken. Login fail." << endl;
    else if( (Cloud::getNodePtrById(userNode) != NULL) // Atention: short circuit
             && (Cloud::getNodePtrById(userNode)->isOnline() == false ) )
      cout << "[" << __LINE__ << "] User name (" << name << ") in node ("
           << userNode << "). The node is down. Login fail." << endl;
    else new User(name, userId, userNode, userLoginDate, nbVMs, vmFamily);
    infile >> name;
  }
}

bool User::userIdTaken(int id) {
  return (usersListById.find(id) != usersListById.end()) ? true : false;
}

bool User::userNameTaken(string name) {
  return (usersListByName.find(name) != usersListByName.end()) ? true : false;
}

User* User::getUserPtrById( int id ) {
  return usersListById[id];
}
User* User::getUserPtrByName( string name ) {
  return usersListByName[name];
}

void User::printAllUsers() {
  for( auto it = usersListByName.begin() ; it != usersListByName.end() ; ++it )
    cout << *it->second << endl;
}

void User::onboarding() {
  User::readUserFile();
}

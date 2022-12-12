#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string.h>

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

User::User( string name, int id, int host, int userLoginDate, int nbVMs, int vmFamily )
     : userName(name), userId(id), userHost(host), userLoginDate(userLoginDate),
       nbVMs(nbVMs), vmFamily(vmFamily) {
  componentName = strdup(__func__);
  ++userCount;
  usersListById.insert(pair<int,User*>(userId,this));
  usersListByName.insert(pair<string,User*>(userName,this));

  if( userLoginDate > GlobalClock::get() ) {
    status = offline;
    new UserLoginEv(this);
  }
  else {
    myHostHost = Cloud::getHostPtrById(userHost);
    if( myHostHost == NULL ) {
      cout << "User(" << userName << ") login fail: Host(" << userHost << ") is down." << endl;
      return;
    }
    rentNewVMs(nbVMs); 
    status = online;
  }
}

void User::billing( int host, int nbInst ) {
 auto it = invoice.find(host);
  if( it == invoice.end() ) invoice.insert(pair<int,int>(host,nbInst));
  else it->second += nbInst;
}

bool User::rentNewVMs( int n ) {
  if( n <= 0 ) return false;
  for( ; n > 0 ; --n ) {
    auto vm = VM::createNewVM(myHostHost,this);
    myVMPool.push_back(vm);
  }
  return true;
}

ostream& operator<<( ostream& out, User& u ) {
  out << "(" << u.getId() << "," << u.getName() << "): " << u.myVMPool.size() << " VMs";
  return out;
}

void User::userLogin() {
  myHostHost = Cloud::getHostPtrById(userHost);
  if( myHostHost == NULL ) {
    cout << "User(" << userName << ") login fail: Host(" << userHost << ") is down." << endl;
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
  int    userId, userHost, userLoginDate, nbVMs, vmFamily;

  std::ifstream infile(userFileName);
  cout << "User file name: " << userFileName << endl;
  
  infile >> name;
  while( !infile.eof() ) {
    infile >> userId >> userHost >> userLoginDate >> nbVMs >> vmFamily;
    if( userIdTaken(userId) || userNameTaken(name) )
      cout << "[" << __LINE__ << "] User name (" << name << ") or user id ("
           << userId << ") already taken. Login fail." << endl;
    else if( (Cloud::getHostPtrById(userHost) != NULL) // Atention: short circuit
             && (Cloud::getHostPtrById(userHost)->isOnline() == false ) )
      cout << "[" << __LINE__ << "] User name (" << name << ") in host ("
           << userHost << "). The host is down. Login fail." << endl;
    else new User(name, userId, userHost, userLoginDate, nbVMs, vmFamily);
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

void User::charge() {
  for( auto it = usersListById.begin() ; it != usersListById.end() ; ++it ) {
    cout << "------------\n";
    cout << "User :\t" << it->first << endl;
    
    for( auto iti = (it->second)->invoice.begin() ; iti != (it->second)->invoice.end() ; ++iti )
      cout << "\t[" << iti->first << "," << iti->second << "]" << endl;
  }
}

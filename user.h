#ifndef __user_h__
#define __user_h__

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "utils.h"
#include "task.h"
#include "virtualmachine.h"
#include "component.h"

using namespace std;

class User : public Component {
  static map<int,User*>    usersListById;
  static map<string,User*> usersListByName;
  static int userCount;
  string userName;
  int    userId, status, userNode, userLoginDate, nbVMs, vmFamily;
  vector<VM*> myVMPool;
  Node *myHostNode;

public:
  User( string name, int id, int node, int userLoginDate, int nbVMs, int vmFamily = 0 );
  bool rentNewVMs( int n = 1 );
  //void schedule( vector<Task*> &taskL );
  inline int getId() const { return userId; }
  inline string getName() const { return userName; }
  inline Node& getNode() const { return *myHostNode; }
  inline int getNodeId() const { return userNode; }
  inline vector<VM*>& getVMPool() { return myVMPool; }
  void userLogin();
  inline bool getStatus() const { return status; }
  inline int getUserLoginDate() const { return userLoginDate; }

  friend ostream& operator<<( ostream& out, User& u );

  static void readUserFile(string userFileName = "input/passwd.pwd");
  static bool userIdTaken(int id);
  static bool userNameTaken(string name);
  static void onboarding();
  static User* getUserPtrById( int id );
  static User* getUserPtrByName( string name );
  static void printAllUsers();
  static bool isOnline(int userId) {
    auto it = usersListById.find(userId);
    if( it == usersListById.end() ) return false;
    else return usersListById[userId]->getStatus();
  }
};

#endif

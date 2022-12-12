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
  map<int,int> invoice;
  static int userCount;
  string userName, userNode;
  int    userId, status, userLoginDate, nbVMs, vmFamily;
  vector<VM*> myVMPool;
  Host *myHostHost;

public:
  User( string name, int id, string node, int userLoginDate, int nbVMs, int vmFamily = 0 );
  bool rentNewVMs( int n = 1 );
  //void schedule( vector<Task*> &taskL );
  inline int getId() const { return userId; }
  inline string getName() const { return userName; }
  inline string getNode() const { return userNode; }
  inline Host& getHost() const { return *myHostHost; }
  inline int getHostId() const { return 99; }
  inline vector<VM*>& getVMPool() { return myVMPool; }
  inline bool getStatus() const { return status; }
  inline int getUserLoginDate() const { return userLoginDate; }
  inline map<int,int>& getInvoice() { return invoice; }

  void userLogin();
  void billing(int host, int nbInst );

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
  static void charge();
};

#endif

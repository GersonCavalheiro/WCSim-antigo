#ifndef __globaldata_h__
#define __globaldata_h__

#include <list>
#include <set>

struct GlobalData {
//  static vector<BoT> bot;
  static vector<Node> cloud;
//  vector<User> users;
  static list<Task*> taskList;
};

//vector<BoT> GlobalData::bot;
vector<Node> GlobalData::cloud;
list<Task*> GlobalData::taskList;

#endif

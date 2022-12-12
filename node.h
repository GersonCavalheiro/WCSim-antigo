#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include <map>
#include <string>

class Host;

using namespace std;

class Node {
  string name;
  int    id;
  static int nodeCount;
  vector<Host*> hostsL;
  static vector<Node*> nodesL;
  static map<string,Node*> nodesM;

  Node(string name);

public:
  static void pushHost(string name, Host *host);
  void pushHost(Host *host);
  static void popHost(Host *host);
  vector<Host*>::iterator getHostsList(string name);
};

#endif

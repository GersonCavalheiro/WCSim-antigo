/* Extract all integers from string */
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bits/stdc++.h>
#include <list>
#include <set>

#include "utils.h"
#include "globalclock.h"
#include "bot.h"
#include "virtualmachine.h"
#include "user.h"
#include "task.h"
#include "node.h"
#include "event.h"
#include "cloud.h"
#include "simulator.h"

using namespace std;

static void startSequence() {
  Cloud::deploy();
  Cloud::printAllCloud();
  User::onboarding();
  BoT::load();
}

int main() {
  startSequence();

  Cloud::printAllCloud();
  User::printAllUsers();
  BoT::printAllBoTs();
  VM::printAllVMs();
  cout << "++++++\n";
  Cloud::printAllLinks();
  cout << "++++++\n";

  Simulator::printAllEvents();

  Simulator::run();

  cout << "++++++\n";
  vector<int> v;
  Cloud::getLinkSpeed(3,v);
  for (int s : v ) 
   cout << s << " ";
  cout << "\n++++++\n";
  cout << "Global::Clock: " << GlobalClock::get() << endl;


  return 0;
}

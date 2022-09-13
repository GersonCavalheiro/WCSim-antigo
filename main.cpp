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
#include "usage.h"

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
  cout << "Global::Clock: " << GlobalClock::get() << endl;
  cout << "Tasks - Total: " << Task::getNbTasks() << ", Running: "
       << Task::getRunningTasks() << ", Completed: "
       << Task::getCompletedTasks() << endl;

  cout << "++++++\n";
  Usage::print();
  User::charge();

  return 0;
}

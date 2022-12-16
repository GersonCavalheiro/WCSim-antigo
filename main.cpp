/* Extract all integers from string */
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bits/stdc++.h>
#include <list>
#include <set>

#include "simulator.h"
#include "event.h"
#include "utils.h"
#include "globalclock.h"
#include "virtualmachine.h"
#include "task.h"
#include "bot.h"
#include "user.h"
#include "host.h"
#include "cloud.h"
#include "schedulepolice.h"
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

//  Cloud::printAllCloud();
//  User::printAllUsers();
//  BoT::printAllBoTs();
//  VM::printAllVMs();
//  Cloud::printAllLinks();

  Simulator::run();

  cout << "++++++\n";
  cout << "Global::Clock: " << GlobalClock::get() << endl;
  cout << "Tasks - Total: " << Task::getNbTasks() << ", Running: "
       << Task::getRunningTasks() << ", Completed: "
       << Task::getCompletedTasks() << endl;

  cout << "++++++\n";
  Usage::print();
  cout << "++++++\n";
  User::charge();
  cout << "++++++\n";
  cout << "Last BoT execution at: " << Simulator::lastBoTFinishDate << endl;

  return 0;
}

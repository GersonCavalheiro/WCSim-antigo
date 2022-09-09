#include <iostream>
#include <string>
#include "event.h"
#include "bot.h"
#include "user.h"
#include "cloud.h"
#include "node.h"
#include "simulator.h"

int Event::eventCount = 0;

// ------- ALL EVENTS
Event::Event( int date, int prior )
   : date(date), prior(prior) {
  eventId = eventCount++;
  Simulator::newEvent(this);
}

bool operator<( const Event &e1, const Event &e2 ) { return e1.date < e2.date; }
bool operator<=( const Event &e1, const Event &e2 ) { return e1.date <= e2.date; }

ostream& operator<<( ostream& out, Event &e ) {
  out << e.eventName() << "(" << e.eventId << "): " << e.getDate();
  return out;
}

// -------- BOT EVENTS 
BoTReadyEv::BoTReadyEv( BoT *bot ) : Event(GlobalClock::get(), 3), bot(bot) {
  bot->pushEvent(this);
}

BoTReadyEv::~BoTReadyEv() {
  bot->popEvent();
}

void BoTReadyEv::execute() {
  cout << "[" << getDate() << "] " << "BoT(" << bot->getId() << ") owner(" << bot->getOwnerPtr()->getName() << "), Node(" << bot->getSourceNodePtr()->getName() << ") ready." << endl;
  bot->launch();
}

BoTFinishEv::BoTFinishEv( BoT *bot ) : Event(GlobalClock::get(), 3), bot(bot) {
  bot->pushEvent(this);
}

void BoTFinishEv::execute() {
  cout << "[" << getDate() << "] " << "BoT(" << bot->getId() << ") owner(" << bot->getOwnerPtr()->getName() << "), Node(" << bot->getSourceNodePtr()->getName() << ") finish." << endl;
  vector<BoT*> succ = bot->getSuccessorsL();
  for( auto it = succ.begin() ; it != succ.end() ; ++it )
    (*it)->dependenceSatisfied();
}

string BoTFinishEv::eventName() {
  return to_string(GlobalClock::get())+string(": BoT(")+to_string(bot->getId())+") completed.";
}

BoTFinishEv::~BoTFinishEv() {
  bot->popEvent();
}

// -------- USER EVENTS 
UserLoginEv::UserLoginEv( User* user )
    : Event(user->getUserLoginDate(),2), user(user) {
  user->pushEvent(this);
}

void UserLoginEv::execute() {
  user->userLogin();
  cout << "[" << getDate() << "] " << "User(" << user->getName() << ") login." << endl;
}

UserLoginEv::~UserLoginEv() {
  user->popEvent();
}

// --------- NODE EVENTS
NodeRisingEv::NodeRisingEv( Node* node ) : Event(node->getRisingDate(),0), node(node) {
  node->pushEvent(this);
}

NodeRisingEv::~NodeRisingEv() {
  node->popEvent();
}

void NodeRisingEv::execute() {
  node->setStatus(online);
  cout << "[" << getDate() << "] " << "Node(" << node->getName() << ") Rising." << endl;
}

string NodeRisingEv::eventName() {
  return to_string(date)+string(": Node(")+node->getName()+string(") Rising.");
}

NodeShutdownEv::NodeShutdownEv( Node* node ) : Event(GlobalClock::get(),0), node(node) {
  node->pushEvent(this);
}

NodeShutdownEv::~NodeShutdownEv() {
  node->popEvent();
}

void NodeShutdownEv::execute() {
  node->setStatus(offline);
  cout << "[" << getDate() << "] " << "Node(" << node->getName() << ") Shutdown." << endl;
}

string NodeShutdownEv::eventName() {
  return to_string(date)+string(": Node")+node->getName()+string(") Shutdown.");
}

// --------- TASK EVENTS
TaskFinishEv::TaskFinishEv( Task *task, int date ) : Event(date,4), task(task) {
  task->pushEvent(this);
}

TaskFinishEv::~TaskFinishEv() { 
  task->popEvent();
}

void TaskFinishEv::execute() {
  task->getVMRunning()->popTask(task);
  cout << "[" << getDate() << "] " << "Task(" << task->getId() << ") BoT(" << task->getBoT()->getId() << ") finish." << endl;
}


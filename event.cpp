#include <iostream>
#include <string>
#include <string.h>
#include "event.h"
#include "bot.h"
#include "user.h"
#include "cloud.h"
#include "host.h"
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
  out << "[" << e.nameEv << "]" << e.eventName()
      << "(" << e.eventId << "): " << e.getDate();
  return out;
}

// -------- BOT EVENTS 
BoTReadyEv::BoTReadyEv( BoT *bot ) : Event(GlobalClock::get(), 3), bot(bot) {
  nameEv = strdup(__func__);
  nameComp = bot->getComponentName();
  idComp   = bot->getId();
  bot->pushEvent(this);
}

BoTReadyEv::~BoTReadyEv() {
  bot->popEvent();
}

void BoTReadyEv::execute() {
  bot->launch();
}

BoTFinishEv::BoTFinishEv( BoT *bot ) : Event(GlobalClock::get(), 3), bot(bot) {
  nameEv = strdup(__func__);
  nameComp = bot->getComponentName();
  idComp   = bot->getId();
  bot->pushEvent(this);
}

void BoTFinishEv::execute() {
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
  nameEv = strdup(__func__);
  nameComp = user->getComponentName();
  idComp   = user->getId();
  user->pushEvent(this);
}

void UserLoginEv::execute() {
  user->userLogin();
  //cout << "[" << getDate() << "] " << "User(" << user->getName() << ") login." << endl;
}

UserLoginEv::~UserLoginEv() {
  user->popEvent();
}

// --------- NODE EVENTS
HostRisingEv::HostRisingEv( Host* host ) : Event(host->getRisingDate(),0), host(host) {
  nameEv = strdup(__func__);
  nameComp = host->getComponentName();
  idComp   = host->getId();
  host->pushEvent(this);
}

HostRisingEv::~HostRisingEv() {
  host->popEvent();
}

void HostRisingEv::execute() {
  host->setStatus(online);
  //cout << "[" << getDate() << "] " << "Host(" << host->getName() << ") Rising." << endl;
}

string HostRisingEv::eventName() {
  return to_string(date)+string(": Host(")+host->getName()+string(") Rising.");
}

HostShutdownEv::HostShutdownEv( Host* host ) : Event(GlobalClock::get(),0), host(host) {
  nameEv = strdup(__func__);
  nameComp = host->getComponentName();
  idComp   = host->getId();
  host->pushEvent(this);
}

HostShutdownEv::~HostShutdownEv() {
  host->popEvent();
}

void HostShutdownEv::execute() {
  host->setStatus(offline);
  //cout << "[" << getDate() << "] " << "Host(" << host->getName() << ") Shutdown." << endl;
}

string HostShutdownEv::eventName() {
  return to_string(date)+string(": Host")+host->getName()+string(") Shutdown.");
}

// --------- TASK EVENTS
TaskFinishEv::TaskFinishEv( Task *task, int date ) : Event(date,4), task(task) {
  nameEv = strdup(__func__);
  nameComp = task->getComponentName();
  idComp   = task->getId();
  task->pushEvent(this);
}

TaskFinishEv::~TaskFinishEv() { 
  task->popEvent();
}

void TaskFinishEv::execute() {
  task->getVMRunning()->popTask(task);
  //cout << "[" << getDate() << "] " << "Task(" << task->getId() << ") BoT(" << task->getBoT()->getId() << ") finish." << endl;
}

InstanceSuspendEv::InstanceSuspendEv(Instance *vm, int date) : Event(date,0), vm(vm) {
  nameEv = strdup(__func__);
  nameComp = vm->getComponentName();
  idComp   = vm->getId();
  vm->pushEvent(this);
}

InstanceSuspendEv::~InstanceSuspendEv() {
  vm->popEvent();
}

void InstanceSuspendEv::execute() {
  cout << "Suspendendo!!!" << endl;
  vm->suspend();
}

InstanceResumeEv::InstanceResumeEv(Instance *vm, int date) : Event(date,0), vm(vm) {
  nameEv = strdup(__func__);
  nameComp = vm->getComponentName();
  idComp   = vm->getId();
  vm->pushEvent(this);
}

InstanceResumeEv::~InstanceResumeEv() {
  vm->popEvent();
}

void InstanceResumeEv::execute() {
  cout << "Retomando!!!" << endl;
  vm->resume();
}

MigrationStartEv::MigrationStartEv(Node *node, int date) : Event(date,0), node(node) {
  nameEv = strdup(__func__);
  nameComp = node->getComponentName();
  idComp   = node->getId();
  node->pushEvent(this);
}

MigrationStartEv::~MigrationStartEv() {
  node->popEvent();
}

void MigrationStartEv::execute() {
  Instance* vm = *(*(node->getHostsList().begin()))->getVMList().begin();
  vm->suspend();
  vm->migrate(7);
  new InstanceResumeEv(vm,3000);
}

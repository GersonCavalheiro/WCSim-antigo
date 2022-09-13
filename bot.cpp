#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <list>

#include "utils.h"
#include "event.h"
#include "task.h"
#include "simulator.h"
#include "bot.h"
#include "user.h"
#include "cloud.h"
#include "scheduler.h"
#include "vmschedule.h"

using namespace std;

vector<BoT*> BoT::botsL;
int          BoT::botCount = 0;

BoT::BoT(vector<int>& attr) : initialAttribs(attr) {
  botIId = botCount++;
  botsL.push_back(this);
  status = waiting_t;
  initialPriority = 0;
  nbRunningTasks = nbCompletedTasks = 0;
  currentDependences = initialAttribs[nDependBoT];
  owner       = User::getUserPtrById(initialAttribs[ownerId]);
  sourceNode  = Node::getNodePtrById(owner->getNodeId());

  for( int i = initialAttribs[nbTasks] ; i > 0 ; --i )
    tasksL.push_back(new Task(this, initialAttribs) );

  if( initialAttribs[nDependBoT] == 0 ) new BoTReadyEv(this);
}

void BoT::aTaskCompleted() {
  //cout << "BoT("<<getId()<<") a task completed\n";
  ++nbCompletedTasks;
  --nbRunningTasks;
  if( nbCompletedTasks == initialAttribs[nbTasks] ) {
    //cout << "BoT("<<getId()<<") solving dependences\n";
    new BoTFinishEv(this);
  }
}

void BoT::dependenceSatisfied() {
  //cout << "Satisfazendo dependencias: " << currentDependences << endl;
  if( --currentDependences > 0 ) return;
  new BoTReadyEv(this);
}

void BoT::launch() {
  status = running_t;
  nbRunningTasks = tasksL.size();
  //owner->schedule(tasksL,VMSchedule::selectVMByNbTasks);
  Scheduler::scheduleTaskOnVM(tasksL,*owner,VMSchedule::selectVMByNbTasks);
}

ostream& operator<<( ostream& out, const BoT& b ) {
  out << "[" << b.initialAttribs[0];
  for( int i = 1 ; i < b.initialAttribs.size() ; ++i ) cout << "," << b.initialAttribs[i];
  cout << "]";
  if( b.successors.size() > 0 ) {
    out << " <- ( ";
    for( auto it = b.successors.begin() ; it != b.successors.end() ; ++it )
      out << (*it)->getBoTId() << " ";
    out << ")";
  }
  return out;
}

bool operator<(const BoT &b1, const BoT &b2 ) {
  if( b1.getArrivalDate() == b2.getArrivalDate() ) return b1.getPriority() < b2.getPriority();
  return b1.getArrivalDate() < b2.getArrivalDate();
}
  
void BoT::printAllBoTs() {
  cout << "---\n";
  for( auto it = botsL.begin() ; it != botsL.end() ; ++it )
    cout << *(*it) << endl;
  cout << "---\n";
}

void BoT::extractBoTAttributes(vector<int>& attr, string& strIn ) {
  stringstream sIn;
  string aux;
  int number, i;

  sIn << strIn;
  // Read first the fixed attributes: botId, ownerId, nodeId, nDependBoT,
  //                                  arrivalBoT, nbTasks,
  //                                  nbInstructions, memTask
  for( i = 0 ; i < nbOfAttrib ; ++i ) { 
    sIn >> aux;
    if( !(stringstream(aux) >> number) ) exit(0);
    attr.push_back(number);
  }
  // The, if exist, the dependences
  i = attr[nDependBoT]; // i = nDepend
  while( i-- > 0 ) {
    sIn >> aux;
    if( !(stringstream(aux) >> number) ) exit(0);
    attr.push_back(number);
  }
}

void BoT::buildBoT( string& attribs ) {
  vector<int> attr;

  if( attribs.size() == 0 ) return;
  extractBoTAttributes(attr, attribs);
  if( User::isOnline(attr[ownerId]) ) new BoT(attr);
  else cout << "BoT " << attr[botId] << " fail on creating: owner("
            << attr[ownerId] << ") is offline" << endl;
}

void BoT::readBoTFile() {
  string strIn, strOut;
  
  std::getline(cin,strIn);
  while( !cin.eof() ) {
    strOut = extractAllIntegers(strIn);
    if( strOut.size() != 0 ) buildBoT(strOut); 
    std::getline(cin,strIn);
  }
}

void BoT::readBoTFile( string botFileName ) {
  string strIn, strOut;
 
  std::ifstream infile(botFileName);
  cout << "DoB file name: " << botFileName << endl;
  std::getline(infile,strIn);
  while( !infile.eof() ) {
    strOut = extractAllIntegers(strIn);
    if( strOut.size() != 0 ) buildBoT(strOut); 
    std::getline(infile,strIn);
  }
}

void BoT::makeBoTDependences() {
  for( auto it = botsL.begin() ; it != botsL.end() ; ++it ) {
    int nb = (*it)->getNbDepend();
    int d = 0;
    while( nb-- > 0 ) botsL[(*it)->getDependence(d++)]->setDependence(*it); 
  }
}

void BoT::load() {
  BoT::readBoTFile("sipht.dob");
  BoT::makeBoTDependences();
}

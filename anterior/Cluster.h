#include "Processor.h"

class Cluster {
private:
  static int clusterId;
  int cId;
  vector<Processor*> procs;
public:
  Cluster( int nbprocs = 4 ) {
    cId = clusterId++;
    procs.resize(nbprocs);
    for( vector<Processor*>::iterator it = procs.begin() ; it != procs.end() ; ++it )
      *it = new Processor();
  }

  void localSchedule( deque<Task>& lTasks, int curStep ) {
    lTasks.pop_front();
  }
};

int Cluster::clusterId = 0;

/*
  void incStep() {
    for( vector<Processador*>::iterator it = procs.begin() ;
         it != procs.end() ; ++it )
      (*it)->incStep();
  } 

  void localSchedule( deque<Task>& lTask, int curStep ) {
    deque<Task>::iterator aux, stepE;

    do { // (6)
      for( vector<Processador*>::iterator it = procs.begin() ;
           it != procs.end() ; ++it ) {
        aux = (*it)->pickOne(lTask.begin(),stepE); // (7)
        if( aux != stepE ) { // (8)
          lTask.erase(aux);
          for( stepE = lTask.begin() ; stepE != lTask.end() && stepE->getTaskGlobalStep() == curStep ; ++stepE );
        }
        else break;
      } 
    } while( lTask.begin() != stepE && lTask.begin() != lTask.end() && maxAvailableLoad() >= minTaskLoad(lTask.begin(),stepE) );
  }

  void globalSchedule( deque<Task>& lTask, int curStep ) {
    deque<Task>::iterator aux, stepE;
  }

private:
  inline int maxAvailableLoad() {
    vector<Processador*>::iterator it = procs.begin();
    int m = (*it)->getLoad();

    for( ++it ; it != procs.end() ; ++it )
      if( m > (*it)->getLoad() ) m = (*it)->getLoad();
    return 100-m;
  }
  inline int minTaskLoad( deque<Task>::iterator b, deque<Task>::iterator e ) {
    int m = 101;
    for( ; b != e ; ++b )
      if( b->getTaskCost() < m ) m = b->getTaskCost();
    return m;
  }
};

*/

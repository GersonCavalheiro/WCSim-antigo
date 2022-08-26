class Grid {
private:
  vector<Cluster*> clus;
  int nbclus;
public:
  Grid( int nbclus = 4, int nbprocs = 4 ) : nbclus(nbclus) {
  clus.resize(nbclus);
    for( vector<Cluster*>::iterator it = clus.begin() ;
         it != clus.end() ; ++it )
      *it = new Cluster(nbprocs);
  }

  void run( vector<deque<Task>>& lTasks ) {
    int curStep = 0, nextStep; // Identifica o step em tratamento
    vector<Cluster*>::iterator itc;
    vector<deque<Task>>::iterator itt;

    cout << "Incompletas: " << Task::incompleteTasks(lTasks) << endl;

    // Enquanto houver jobs para serem escalonados
    for( curStep = 0 ; Task::incompleteTasks(lTasks) ; ++curStep ) {
      for( itc = clus.begin(), itt = lTasks.begin() ; itc != clus.end() ; ++itc, ++itt )
        (*itc)->localSchedule(*itt,curStep);
    }

/*
      nextStep = lTasks.begin()->getTaskGlobalStep(); // Identifica o tempo de chegada do primeiro nao ainda executado
      for( stepE = lTasks.begin() ; stepE != lTasks.end() && stepE->getTaskGlobalStep() == nextStep ; ++stepE ); // Identica todos os jobs no mesmo step
      if( curStep < nextStep )  // Se houve um salto de tempo (steps) sem job, atualiza a carga de todos os processadores com 0 (zero) nos steps sem trabalho
        while( lTasks.begin()->getTaskGlobalStep() > nextStep ) {
          ++curStep;
          for( vector<Cluster*>::iterator it = clus.begin() ;
               it != clus.end() ; ++it )
            (*it)->incStep();
        }

      for( i = 0 ; i < nbclus ; ++i ) Task::sortTaskList(lTasks[i].begin(), stepE, byCost()); // (5)

      for( it = clus.begin(), i = 0 ; it != clus.end() ; ++it, ++i )
        (*it)->localSchedule(lTasks[i],curStep);
      for( it = clus.begin(), i = 0 ; it != clus.end() ; ++it, ++i )
        (*it)->globalSchedule(lTasks[i], curStep);
  } 
*/
 }
};


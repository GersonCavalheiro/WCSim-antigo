struct byArrivalTime;

class Task { // w = (g,r,f,c,m) 
  int myTaskNumber, // Tasks owning this job
      a,  // Arival time of the task
      d,  // Number of jobs in the task
      g,  // Global step
      f,  // The rank of the job in the task
      r,  // Number of jobs after the current
      c,  // Cost of the job
      m,  // Cost to communicate the job
      n,  // Source node
      delay; // Step delayed

  public:
  Task(int t, int a, int d, int c, int m, int n ) {
    this->myTaskNumber =  t;
    this->a = a; this->d = d;
    this->g = a;
    this->r = d-(g-a);
    this->f = f;
    this->c = c;
    this->m = m;
    this->n = n;
    this->delay = 0;
  }

  inline int  getTaskId() const { return myTaskNumber; }
  inline int  getTaskArrival() const { return a; }
  inline int  getTaskStep() const { return f; } // the current job
  inline int  getTaskCost() const { return c; } // c
  inline int  getTaskCommCost() const { return m; } // m
  inline int  getTaskSourceNode() const { return n; } // n
  inline int  getTaskDuration() const { return d; }
  inline int  getTaskFinished() const { return g-a; } // f
  inline int  getTaskRemaining() const { return d-(g-a); } // r
  inline int  getTaskGlobalStep() const { return g; }
  inline void incTaskGlobalStep() { ++a; ++g; }
  inline void incTaskDelay() { ++delay; }

  friend bool operator<(const Task &t1, const Task &t2 );

  friend ostream& operator<<( ostream &out, const Task &t ) {
    out << "[" << t.myTaskNumber << "," << t.a << "]"
        << "(" << t.g << "," << t.r << "," << t.f << "," << t.c << ")";
    return out;
  }
  // Sort all tasks: by global step followed by a criteria
  
  template<class T = byArrivalTime>
  static inline void sortAll( vector<deque<Task>> &l, T priority) {
   for( vector<deque<Task>>::iterator it = l.begin() ;
        it != l.end() ; ++it )
      sort(it->begin(), it->end(), priority);
  }

  template<class T = byArrivalTime>
  static inline void sortAll_antigo( vector<deque<Task>> &l) {
   for( vector<deque<Task>>::iterator it = l.begin() ;
        it != l.end() ; ++it )
      sort(it->begin(), it->end());
  }

  template<class T = byArrivalTime>
  static inline void sortTaskList( deque<Task>::iterator b, deque<Task>::iterator e, T priority ) {
    sort(b,e,priority);
  }

  static void buildTaskList( vector<deque<Task>> &lTasks) {
    int a, d, q, c, m, n;
    int lines = 0; // Task id

    lines = 0; // counts the number of lines in the .bot file // lines+1 is the current task number
    for( cin >> a >> d >> q >> c >> m >> n ; !cin.eof() ; cin >> a >> d >> q >> c >> m >> n , ++lines ) {
      // Each line describe a group of tasks: [a d q c]
      // a: arrival time of the entire group
      // d: duration of each task
      // q: amount of tasks in the group
      // c: computational cost of each task
      // a, d: represented in steps
      // c: percetual of CPU utilization
      // m: amount of memory / communication cost
      // n: cluster of origin

      lTasks.at(n).push_back(Task(lines,a,d,c,m,n));
    }
  }
 
  static int incompleteTasks( vector<deque<Task>> &lTasks) {
    int incomplete = 0;

    for( vector<deque<Task>>::iterator it = lTasks.begin() ; it != lTasks.end() ; ++it )
      incomplete += it->size();

    return incomplete;
  }

  static void printTaskList( vector<deque<Task>> &lTasks) {
    for( int i = 0 ; i < NODES ; ++i ) {
      cout << "Node: " << i << ": ";
      for( deque<Task>::iterator it = lTasks.at(i).begin() ; it != lTasks.at(i).end() ; ++it )
        cout << it->getTaskId() << ",";
      cout << endl;
    }
  }
};

//Comparador default
bool operator<(const Task &t1, const Task &t2 ) {
    if( t1.getTaskArrival() == t2.getTaskArrival() )
      return t1.getTaskId() < t2.getTaskId();
    return t1.getTaskArrival() < t2.getTaskArrival();
}

struct byNone {
  bool operator()( const Task &t1, const Task &t2 ) {
    if( t1.getTaskArrival() == t2.getTaskArrival() )
      return t1.getTaskId() < t2.getTaskId();
    return t1.getTaskArrival() < t2.getTaskArrival();
  }
};

struct byCost {
  bool operator()( const Task &t1, const Task &t2 ) {
    if( t1.getTaskArrival() == t2.getTaskArrival() )
      return t1.getTaskCost() < t2.getTaskCost();
    else return t1.getTaskArrival() < t2.getTaskArrival();
  }
};


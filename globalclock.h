#ifndef __global_clock_h__
#define __global_clock_h__

class GlobalClock {
private:
  static int clock;
public:
  GlobalClock() {}
  static int set( int t ) { return clock = t; }
  static int get() { return clock; }
};
#endif

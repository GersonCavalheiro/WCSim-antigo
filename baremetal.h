#ifndef __baremetal_h__
#define __baremetal_h__

#include <climits>
#include "virtualmachine.h"

class BareMetal {
private:
  int  cores, speed, ram, storage, gpu;
  int  occupedCores, observedSpeed, occupedRam, occupedStorage;
  bool pinnedGpu;
public:
  /* Default:
   *         4 cores            (number of cores)
   *         10 MPIS per core   (speed)
   *         64 GB RAM Total    (4 GB per core)
   *         unlimited storage  (INT_MAX TB)
   *         GPU non avalable   (false)
  */
  BareMetal( int cores = 4, int speed = 10, int ram = 16, int storage = INT_MAX, bool gpu = false )
       : cores(cores), speed(speed),
         ram(ram*cores), storage(storage), gpu(gpu) {
    occupedRam = occupedCores = occupedStorage = 0;
    pinnedGpu = false;
  }
  inline virtual int  getCores() const { return cores; }
  inline virtual int  getSpeed() const { return speed; }
  inline virtual int  getRam() const { return ram; }
  inline virtual bool getGPU() const { return false; }
  inline virtual int  getStorage() const { return storage; }
  virtual void pinCore();
  virtual void releaseCore();
  virtual void place( VM *vm );
  virtual void unplace( VM *vm );
  virtual bool fitRam( VM *vm );
};

#endif



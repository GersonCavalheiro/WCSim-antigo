#ifndef __baremetal_h__
#define __baremetal_h__

#include <climits>
#include "component.h"
#include "virtualmachine.h"
#include "usage.h"

class BareMetal : public Component {
private:
  int  cores, mips, ram, storage, gpu;
  int  occupedCores, occupedRam, occupedStorage;
  int  lastDataStamp;
  bool pinnedGpu;
public:
  /* Default:
   *         4 cores            (number of cores)
   *         10 MPIS per core   (speed)
   *         64 GB RAM Total    (4 GB per core)
   *         unlimited storage  (INT_MAX TB)
   *         GPU non avalable   (false)
  */
  BareMetal( int cores = 4, int mips = 100000, int ram = 16, int storage = INT_MAX, bool gpu = false )
       : cores(cores), mips(mips),
         ram(ram*cores), storage(storage), gpu(gpu) {
    lastDataStamp = occupedRam = occupedCores = occupedStorage = 0;
    pinnedGpu = false;
  }
  inline virtual int   getId() const = 0;
  inline virtual int   getCores() const
                           { return cores; }
  inline virtual int   getMips() const
                           { return mips; }
  inline virtual int   getRam() const
                           { return ram; }
  inline virtual bool  getGPU() const
                           { return false; }
  inline virtual int   getStorage() const
                           { return storage; }
  inline virtual float utilizationRate() const
                           { return (cores >= occupedCores)
				    ? 1.0
				    : (float)cores/occupedCores; }
  inline virtual int   getActualMips() const
                           { return mips*this->utilizationRate(); }
  inline virtual int   miDelivered(int t) const
                           { return this->utilizationRate()*mips*t; }
  virtual void pinCore() {
                  Usage::update(this->getId(),
				GlobalClock::get()-getDataStamp(),
				(float)occupedCores/cores);
	          ++occupedCores;
		  setDataStamp();
  }

  virtual void releaseCore() {
                  Usage::update(this->getId(),
				GlobalClock::get()-getDataStamp(),
				(float)occupedCores/cores);
	          --occupedCores;
		  setDataStamp();
  }

  virtual void place( VM *vm );
  virtual void unplace( VM *vm );
  virtual bool fitRam( VM *vm );
  void setDataStamp()
	  { lastDataStamp = GlobalClock::get(); }
  int  getDataStamp() { return lastDataStamp; }
};

#endif



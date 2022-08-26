#ifndef __baremetal_h__
#define __baremetal_h__

#include <climits>

class BareMetal {
private:
  int coresT, speed, ramT, storageT, gpu;
public:
  /* Default:
   *         4 cores            (number of cores)
   *         10 MPIS per core   (speed)
   *         64 GB RAM Total    (4 GB per core)
   *         unlimited storage  (INT_MAX TB)
   *         GPU non avalable   (false)
  */
  BareMetal( int cores = 4, int speed = 10, int ram = 16, int storage = INT_MAX, bool gpu = false )
       : coresT(cores), speed(speed), ramT(ram*cores), storageT(storage), gpu(gpu) {}
  inline virtual int  getCores() const { return coresT; }
  inline virtual int  getSpeed() const { return speed; }
  inline virtual int  getRam() const { return ramT; }
  inline virtual bool getGPU() const { return false; }
  inline virtual int  getStorage() const { return storageT; }
};

#endif



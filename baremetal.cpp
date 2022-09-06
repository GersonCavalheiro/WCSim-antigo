#include "baremetal.h"
#include "virtualmachine.h"

void BareMetal::place( VM *vm ) { occupedRam += vm->getVRam(); }
void BareMetal::unplace( VM *vm ) { occupedRam -= vm->getVRam(); }
bool BareMetal::fitRam( VM *vm ) { return (ram-occupedRam) >= vm->getVRam(); }

void BareMetal::pinCore() {
  if( ++occupedCores > cores ) {
    //observedSpeed = (int)(((float)occupedCores*speed)/occupedCores);
    observedSpeed = (int)(((float)cores*speed)/occupedCores);
//    cout << "Observed Speed ========= " << observedSpeed << endl;
    updateSpeed();
  }
}

void BareMetal::releaseCore() {
  if( --occupedCores <= cores ) {
    observedSpeed = speed;
 //   cout << "Observed peed (original) ========= " << observedSpeed << endl;
    updateSpeed();
  }
}


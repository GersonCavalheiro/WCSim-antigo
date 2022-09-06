#include "baremetal.h"
#include "virtualmachine.h"

void BareMetal::place( VM *vm ) { occupedRam += vm->getVRam(); }
void BareMetal::unplace( VM *vm ) { occupedRam -= vm->getVRam(); }
bool BareMetal::fitRam( VM *vm ) { return (ram-occupedRam) >= vm->getVRam(); }

void BareMetal::pinCore() {
  if( ++occupedCores > cores ) {
    observedSpeed = (int)((float)speed*((float)cores/occupedCores));
  }
}

void BareMetal::releaseCore() {
  if( --occupedCores <= cores ) {
    observedSpeed = speed;
  }
}


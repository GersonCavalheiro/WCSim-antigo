#include "baremetal.h"
#include "virtualmachine.h"

void BareMetal::place( Instance *vm ) { occupedRam += vm->getVRam(); }
void BareMetal::unplace( Instance *vm ) { occupedRam -= vm->getVRam(); }
bool BareMetal::fitRam( Instance *vm ) { return (ram-occupedRam) >= vm->getVRam(); }


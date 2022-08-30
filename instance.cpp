#include "instance.h"
#include "task.h"

void Instance::place( Task *t ) { occupedVRam += t->getRam(); }
void Instance::unplace( Task *t ) { occupedVRam -= t->getRam(); }
bool Instance::fitRam( Task *t ) { return (vRam-occupedVRam) >= t->getRam(); }


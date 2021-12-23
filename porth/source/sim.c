#include "porth/sim.h"

#include <assert.h>

TOOLBOX_VECTOR_DECLARE(IntVec, int64_t);
TOOLBOX_VECTOR_IMPLEMENT(IntVec, int64_t,
                         TOOLBOX_VECTOR_DESTRUCTOR_NONE(IntVec));

void SimulateProgram(Iota* ops, OpVec* program) {
  assert(false && "not implemented");
}

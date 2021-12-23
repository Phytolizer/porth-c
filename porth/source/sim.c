#include "porth/sim.h"

#include <assert.h>
#include <stdio.h>

TOOLBOX_VECTOR_DECLARE(IntVec, int64_t);
TOOLBOX_VECTOR_IMPLEMENT(IntVec, int64_t,
                         TOOLBOX_VECTOR_DESTRUCTOR_NONE(IntVec));

void SimulateProgram(Iota* ops, OpVec* program) {
  IntVec stack;
  IntVec_init(&stack);
  for (size_t i = 0; i < program->length; ++i) {
    Op* op = program->data[i];
    if (op->type == IotaValue(ops, "OP_PUSH")) {
      OpPush* push = (OpPush*)op;
      IntVec_push(&stack, push->value);
    } else if (op->type == IotaValue(ops, "OP_PLUS")) {
      int64_t b = IntVec_pop(&stack);
      int64_t a = IntVec_pop(&stack);
      IntVec_push(&stack, a + b);
    } else if (op->type == IotaValue(ops, "OP_DUMP")) {
      int64_t val = IntVec_pop(&stack);
      printf("%ld\n", val);
    } else {
      assert(false && "Unknown op type");
    }
  }

  IntVec_deinit(&stack);
}

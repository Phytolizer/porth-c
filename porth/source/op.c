#include "porth/op.h"

#include "nonstd/strdup.h"

TOOLBOX_VECTOR_IMPLEMENT(OpVec, Op*, TOOLBOX_VECTOR_DESTRUCTOR(OpVec, OpFree));

Op* OpCreatePush(Iota* ops, int64_t value) {
  OpPush result = {
      .base =
          {
              .type = IotaValue(ops, "OP_PUSH"),
              .ops = ops,
          },
      .value = value,
  };
  return nonstd_memdup(&result, sizeof(result));
}

Op* OpCreatePlus(Iota* ops) {
  Op result = {
      .type = IotaValue(ops, "OP_PLUS"),
      .ops = ops,
  };
  return nonstd_memdup(&result, sizeof(result));
}

Op* OpCreateDump(Iota* ops) {
  Op result = {
      .type = IotaValue(ops, "OP_DUMP"),
      .ops = ops,
  };
  return nonstd_memdup(&result, sizeof(result));
}

void OpFree(Op* op) {
  // op does not own its iota
  free(op);
}

#pragma once

#include <iota.h>
#include <stdint.h>
#include <toolbox/vector.h>

typedef struct {
  size_t type;
  Iota* ops;
} Op;

TOOLBOX_VECTOR_DECLARE(OpVec, Op*);

typedef struct {
  Op base;
  int64_t value;
} OpPush;

Op* OpCreatePush(Iota* ops, int64_t value);
Op* OpCreatePlus(Iota* ops);
Op* OpCreateMinus(Iota* ops);
Op* OpCreateDump(Iota* ops);
void OpFree(Op* op);

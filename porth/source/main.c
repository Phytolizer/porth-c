#include <config.h>
#include <iota.h>
#include <stdio.h>

#include "porth/op.h"
#include "porth/sim.h"

int main(int argc, char** argv) {
  Iota ops;
  IotaInit(&ops);
  if (!IotaLoad(&ops, PROJECT_SOURCE_DIR "/porth/iota/ops.iota")) {
    return 1;
  }
  printf("Loaded %zu ops\n", ops.count);

  OpVec program;
  OpVec_init(&program);
  OpVec_push(&program, OpCreatePush(&ops, 34));
  OpVec_push(&program, OpCreatePush(&ops, 35));
  OpVec_push(&program, OpCreatePlus(&ops));
  OpVec_push(&program, OpCreateDump(&ops));
  OpVec_push(&program, OpCreatePush(&ops, 500));
  OpVec_push(&program, OpCreatePush(&ops, 80));
  OpVec_push(&program, OpCreateMinus(&ops));
  OpVec_push(&program, OpCreateDump(&ops));

  SimulateProgram(&ops, &program);

  OpVec_deinit(&program);
  IotaDeinit(ops);
}

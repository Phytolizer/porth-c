#include <config.h>
#include <iota.h>
#include <stdio.h>

#include "porth/com.h"
#include "porth/op.h"
#include "porth/sim.h"

static void PrintUsage(char* argv0);

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

  if (argc < 2) {
    PrintUsage(argv[0]);
    fprintf(stderr, "ERROR: no subcommand provided\n");
    return 1;
  }

  char* subcommand = argv[1];
  if (strcmp(subcommand, "sim") == 0) {
    SimulateProgram(&ops, &program);
  } else if (strcmp(subcommand, "com") == 0) {
    CompileProgram(&ops, &program);
  } else {
    PrintUsage(argv[0]);
    fprintf(stderr, "ERROR: unknown subcommand %s\n", subcommand);
    return 1;
  }

  OpVec_deinit(&program);
  IotaDeinit(ops);
}

void PrintUsage(char* argv0) {
  fprintf(stderr, "Usage: %s <SUBCMD> [ARGS]\n", argv0);
  fprintf(stderr, "SUBCMD:\n");
  fprintf(stderr, "   sim     Simulate the program\n");
  fprintf(stderr, "   com     Compile the program\n");
}

#include <config.h>
#include <iota.h>
#include <ops_iota.h>
#include <stdio.h>
#include <subprocess.h>

#include "porth/com.h"
#include "porth/op.h"
#include "porth/sim.h"

static void PrintUsage(char* argv0);
static int SpawnProcess(const char** argv);

int main(int argc, char** argv) {
  Iota ops;
  IotaInit(&ops);
  if (!IotaLoad(&ops, ops_iota, ops_iota_size)) {
    return 1;
  }

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
    CompileProgram(&ops, &program, "output.asm");
    const char* nasm_command_line[] = {
        "/usr/bin/nasm", "-f", "elf64", "output.asm", NULL,
    };
    const char* ld_command_line[] = {
        "/usr/bin/ld", "-o", "output", "output.o", NULL,
    };
    if (SpawnProcess(nasm_command_line) != 0) {
      return 1;
    }
    if (SpawnProcess(ld_command_line) != 0) {
      return 1;
    }
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

int SpawnProcess(const char** argv) {
  struct subprocess_s process;
  int ret = subprocess_create(
      argv,
      subprocess_option_no_window | subprocess_option_combined_stdout_stderr,
      &process);
  if (ret != 0) {
    fprintf(stderr, "ERROR: subprocess creation failed!\n");
    return 1;
  }
  int return_code;
  ret = subprocess_join(&process, &return_code);
  if (ret != 0) {
    fprintf(stderr, "ERROR: subprocess joining failed!\n");
    return 1;
  }
  FILE* err = subprocess_stderr(&process);
  char errbuf[1024];
  int nread;
  while (err != NULL && (nread = fread(errbuf, 1, sizeof(errbuf), err)) != 0) {
    fwrite(errbuf, 1, nread, stderr);
  }
  if (return_code != 0) {
    fprintf(stderr, "ERROR: subprocess exited with code %d\n", return_code);
    return 1;
  }
  subprocess_destroy(&process);
  return 0;
}

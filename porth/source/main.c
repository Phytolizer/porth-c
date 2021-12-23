#include <config.h>
#include <iota.h>
#include <stdio.h>

int main(int argc, char** argv) {
  Iota ops;
  IotaInit(&ops);
  if (!IotaLoad(&ops, PROJECT_SOURCE_DIR "/porth/iota/ops.iota")) {
    return 1;
  }
  printf("Loaded %zu ops\n", ops.count);

  IotaDeinit(ops);
}

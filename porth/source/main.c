#include <config.h>
#include <iota.h>
#include <stdio.h>

int main(int argc, char** argv) {
  Iota i;
  IotaInit(&i);
  if (!IotaLoad(&i, PROJECT_SOURCE_DIR "/porth/iota/ops.iota")) {
    return 1;
  }

  IotaDeinit(i);
}

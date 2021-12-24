#include "porth/com.h"

#include <assert.h>
#include <errno.h>
#include <main_asmt.h>
#include <stdio.h>
#include <string.h>

void CompileProgram(Iota* ops, OpVec* program, const char* out_path) {
  FILE* fp = fopen(out_path, "w");
  if (!fp) {
    fprintf(stderr, "Failed to open %s for writing: %s\n", out_path,
            strerror(errno));
    return;
  }
  fwrite(main_asmt, sizeof(char), main_asmt_size, fp);
  fclose(fp);
}

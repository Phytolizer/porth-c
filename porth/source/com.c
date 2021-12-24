#include "porth/com.h"

#include <assert.h>
#include <errno.h>
#include <prefix_asmt.h>
#include <stdio.h>
#include <string.h>
#include <suffix_asmt.h>

#include "porth/op.h"

void CompileProgram(Iota* ops, OpVec* program, const char* out_path) {
  FILE* fp = fopen(out_path, "w");
  if (!fp) {
    fprintf(stderr, "Failed to open %s for writing: %s\n", out_path,
            strerror(errno));
    return;
  }
  fwrite(prefix_asmt, sizeof(char), prefix_asmt_size, fp);
  for (size_t i = 0; i < program->length; i++) {
    Op* op = program->data[i];
    assert(ops->count == 4 && "Exhaustive handling of ops");
    if (op->type == IotaValue(ops, "OP_PUSH")) {
      OpPush* push_op = (OpPush*)op;
      fprintf(fp, "    ;; -- push %ld --\n", push_op->value);
      fprintf(fp, "    push %ld\n", push_op->value);
    } else if (op->type == IotaValue(ops, "OP_PLUS")) {
      fprintf(fp, "    ;; -- plus --\n");
      fprintf(fp, "    ;; -- TODO: not implemented --\n");
    } else if (op->type == IotaValue(ops, "OP_MINUS")) {
      fprintf(fp, "    ;; -- minus --\n");
      fprintf(fp, "    ;; -- TODO: not implemented --\n");
    } else if (op->type == IotaValue(ops, "OP_DUMP")) {
      fprintf(fp, "    ;; -- dump --\n");
      fprintf(fp, "    ;; -- TODO: not implemented --\n");
    } else {
      assert(false && "unreachable");
    }
  }
  fwrite(suffix_asmt, sizeof(char), suffix_asmt_size, fp);
  fclose(fp);
}

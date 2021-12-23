#include "iota.h"

#include <stdio.h>
#include <string.h>

TOOLBOX_VECTOR_IMPLEMENT(IotaEntryVec, IotaEntry,
                         TOOLBOX_VECTOR_DESTRUCTOR(IotaEntryVec,
                                                   IotaEntryDeinit));

TOOLBOX_HASH_IMPLEMENT(IotaMap, size_t,
                       TOOLBOX_HASH_DESTRUCTOR_KEY(IotaMap, free),
                       TOOLBOX_HASH_MAX_LOAD_DEFAULT);

void IotaEntryDeinit(IotaEntry entry) { free(entry.name); }

void IotaInit(Iota* i) {
  IotaEntryVec_init(&i->entries);
  IotaMap_init(&i->map);
}

bool IotaLoad(Iota* i, const char* filename) {
  FILE* fp = fopen(filename, "r");
  if (!fp) {
    return false;
  }

  char* line = NULL;
  size_t line_size = 0;
  while (getline(&line, &line_size, fp) != -1) {
    if (line_size >= 1 && line[0] == '%') {
      char* savep;
      char* directive = strtok_r(line, " \t", &savep);
      if (directive == NULL) {
        fprintf(stderr, "Invalid directive: %s\n", line);
        free(line);
        return false;
      }

      if (strcmp(directive, "prefix") == 0) {
        char* prefix = strtok_r(NULL, " \t", &savep);
        if (prefix == NULL) {
          fprintf(stderr, "%%prefix requires an argument\n");
          free(line);
          return false;
        }
      }
    }
    free(line);
    line = NULL;
  }
  return false;
}

void IotaDeinit(Iota i) {
  IotaEntryVec_deinit(&i.entries);
  IotaMap_deinit(&i.map);
}

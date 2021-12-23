#include "iota.h"

#include <nonstd/strdup.h>
#include <nonstd/strtok.h>
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

  char* iota_prefix = NULL;
  size_t iota = 0;

  char* line = NULL;
  size_t line_size = 0;
  while (getline(&line, &line_size, fp) != -1) {
    if (line_size == 0) {
      free(line);
      line = NULL;
      continue;
    }
    *strrchr(line, '\n') = '\0';
    if (line[0] == '%') {
      char* savep;
      char* directive = nonstd_strtok_r(line, " ", &savep);
      if (directive == NULL) {
        fprintf(stderr, "Invalid directive: %s\n", line);
        free(line);
        return false;
      }

      if (strcmp(&directive[1], "prefix") == 0) {
        char* prefix = nonstd_strtok_r(NULL, " ", &savep);
        if (prefix == NULL) {
          fprintf(stderr, "%%prefix requires an argument\n");
          free(line);
          return false;
        }
        iota_prefix = nonstd_strdup(prefix);
      } else {
        fprintf(stderr, "Unrecognized directive: %s\n", line);
        free(line);
        return false;
      }
    } else {
      char* name;
      size_t name_len;
      if (iota_prefix != NULL) {
        name_len = strlen(iota_prefix) + strlen(line) + 1;
        name = malloc(name_len);
        strcpy(name, iota_prefix);
        strcat(name, line);
      } else {
        name_len = strlen(line);
        name = nonstd_strdup(line);
      }
      IotaEntryVec_push(
          &i->entries, (IotaEntry){.name = nonstd_strdup(name), .value = iota});
      IotaMap_insert(&i->map, (IotaMap_key){.key = name, .len = name_len},
                     iota);
      ++iota;
    }
    free(line);
    line = NULL;
  }

  free(iota_prefix);
  return true;
}

void IotaDeinit(Iota i) {
  IotaEntryVec_deinit(&i.entries);
  IotaMap_deinit(&i.map);
}

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

static void IotaAdd(Iota* i, const char* line, const char* iota_prefix,
                    size_t iota);

void IotaEntryDeinit(IotaEntry entry) { free(entry.name); }

void IotaInit(Iota* i) {
  IotaEntryVec_init(&i->entries);
  IotaMap_init(&i->map);
}

bool IotaLoad(Iota* i, const char* text, size_t length) {
  char* savep;
  char* iota_prefix = NULL;
  size_t iota = 0;

  char* text_dup = nonstd_strdup(text);

  for (char* line = nonstd_strtok_r(text_dup, "\n", &savep); line != NULL;
       line = nonstd_strtok_r(NULL, "\n", &savep)) {
    if (strlen(line) == 0) {
      continue;
    }
    if (line[0] == '%') {
      char* savep;
      char* directive = nonstd_strtok_r(line, " ", &savep);
      if (directive == NULL) {
        fprintf(stderr, "Invalid directive: %s\n", line);
        return false;
      }

      if (strcmp(&directive[1], "prefix") == 0) {
        char* prefix = nonstd_strtok_r(NULL, " ", &savep);
        if (prefix == NULL) {
          fprintf(stderr, "%%prefix requires an argument\n");
          return false;
        }
        iota_prefix = nonstd_strdup(prefix);
      } else {
        fprintf(stderr, "Unrecognized directive: %s\n", line);
        return false;
      }
    } else {
      if (strcmp(line, "COUNT") == 0) {
        fprintf(stderr, "\"COUNT\" is reserved\n");
        return false;
      }
      IotaAdd(i, line, iota_prefix, iota);
      ++iota;
    }
    line = NULL;
  }

  IotaAdd(i, "COUNT", iota_prefix, iota);
  i->count = iota;

  free(iota_prefix);
  free(text_dup);
  return true;
}

size_t IotaValue(Iota* i, char* name) {
  return *IotaMap_lookup(&i->map,
                         (IotaMap_key){.key = name, .len = strlen(name)});
}

void IotaDeinit(Iota i) {
  IotaEntryVec_deinit(&i.entries);
  IotaMap_deinit(&i.map);
}

void IotaAdd(Iota* i, const char* line, const char* iota_prefix, size_t iota) {
  char* name;
  size_t name_len;
  if (iota_prefix != NULL) {
    name_len = strlen(iota_prefix) + strlen(line);
    name = malloc(name_len + 1);
    strcpy(name, iota_prefix);
    strcat(name, line);
  } else {
    name_len = strlen(line);
    name = nonstd_strdup(line);
  }
  IotaEntryVec_push(&i->entries,
                    (IotaEntry){.name = nonstd_strdup(name), .value = iota});
  IotaMap_insert(&i->map, (IotaMap_key){.key = name, .len = name_len}, iota);
}

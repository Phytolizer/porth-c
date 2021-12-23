#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <toolbox/hash.h>
#include <toolbox/vector.h>

typedef struct {
  char* name;
  size_t value;
} IotaEntry;

TOOLBOX_VECTOR_DECLARE(IotaEntryVec, IotaEntry);
TOOLBOX_HASH_DECLARE(IotaMap, size_t);

typedef struct {
  IotaEntryVec entries;
  IotaMap map;
} Iota;

void IotaEntryDeinit(IotaEntry entry);

void IotaInit(Iota* i);
bool IotaLoad(Iota* i, const char* filename);
void IotaDeinit(Iota i);

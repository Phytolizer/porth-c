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
  size_t count;
} Iota;

void IotaEntryDeinit(IotaEntry entry);

void IotaInit(Iota* i);
bool IotaLoad(Iota* i, const char* text, size_t length);
size_t IotaValue(Iota* i, char* name);
void IotaDeinit(Iota i);

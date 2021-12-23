#include "iota.h"

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

bool IotaLoad(Iota* i, const char* filename) { return false; }

void IotaDeinit(Iota i) {
  IotaEntryVec_deinit(&i.entries);
  IotaMap_deinit(&i.map);
}

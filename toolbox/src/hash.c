#include "toolbox/hash.h"

#define FNV_PRIME 0x00000100000001B3
#define FNV_OFFSET_BASIS 0xCBF29CE484222325

uint64_t ToolboxHashFnv1a(const void* data, size_t len)
{
    uint64_t h = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < len; ++i)
    {
        h ^= ((const char*)data)[i];
        h *= FNV_PRIME;
    }
    return h;
}

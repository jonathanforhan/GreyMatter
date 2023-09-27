#include "memory.hpp"

#define S_MASK (1 << 15)
#define M_MASK (1 << 14)
#define L_MASK (1 << 13)

namespace gml {

static void *get_block(registry_key key, size_t *chunk_size)
{
      if (key & S_MASK) {
        *chunk_size = GML_S_CHUNK_SIZE;
        return sblock;
    } if (key & M_MASK) {
        *chunk_size = GML_M_CHUNK_SIZE;
        return mblock;
    } if (key & L_MASK) {
        *chunk_size = GML_L_CHUNK_SIZE;
        return lblock;
    }
}

static const bool *get_registry(registry_key key)
{
      if (key & S_MASK) {
        return sregistry;
    } if (key & M_MASK) {
        return mregistry;
    } if (key & L_MASK) {
        return lregistry;
    }
}

registry_key reserve_chunk(size_t nbytes)
{
    bool *registry;
    int mask, n;

    if (nbytes <= GML_S_CHUNK_SIZE) {
        registry = sregistry;
        mask = S_MASK;
        n = GML_S_CHUNK_NUM;
    } else if (nbytes <= GML_M_CHUNK_SIZE) {
        registry = mregistry;
        mask = M_MASK;
        n = GML_M_CHUNK_NUM;
    } else if (nbytes <= GML_L_CHUNK_SIZE) {
        registry = lregistry;
        mask = L_MASK;
        n = GML_M_CHUNK_NUM;
    } else {
        return -1;
    }

    
    for (int i = 0; i < n; i++)
    {
        if (!registry[i])
        {
            registry[i] = true;
            return i | mask;
        }
        i++;
    }

    return -1;
}

void *access_chunk(registry_key key)
{
    if (key == -1) return nullptr;

    size_t chunk_size;
    void *block = get_block(key, &chunk_size);

    return block + key * chunk_size;
}

void free_chunk(registry_key key)
{
    if (key == -1) return nullptr;
    bool *registry = get_registry(key);
    registry[key & ~(S_MASK | M_MASK | L_MASK)] = false;
}

size_t get_chunk_size(registry_key key)
{
      if (key & S_MASK) {
        return GML_S_CHUNK_SIZE;
    } if (key & M_MASK) {
        return GML_M_CHUNK_SIZE;
    } if (key & L_MASK) {
        return GML_L_CHUNK_SIZE;
    }
    return 0;
}

} // namespace gml

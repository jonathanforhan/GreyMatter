/* NOT PART OF LIBC++ SPEC THIS IS GREY MATTER SPECIFIC */
#pragma once
#include <Arduino.h>

#ifndef GML_S_CHUNK_SIZE
#   define GML_S_CHUNK_SIZE 64
#endif
#ifndef GML_S_CHUNK_NUM
#   define GML_S_CHUNK_NUM 16
#endif
#ifndef GML_M_CHUNK_SIZE
#   define GML_M_CHUNK_SIZE 128
#endif
#ifndef GML_M_CHUNK_NUM
#   define GML_M_CHUNK_NUM 8
#endif
#ifndef GML_L_CHUNK_SIZE
#   define GML_L_CHUNK_SIZE 256
#endif
#ifndef GML_L_CHUNK_NUM
#   define GML_L_CHUNK_NUM 8
#endif

namespace gml {

using registry_key = int;

static unsigned char sblock[GML_S_CHUNK_SIZE * GML_S_CHUNK_NUM];
static bool sregistry[GML_S_CHUNK_NUM];

static unsigned char mblock[GML_M_CHUNK_SIZE * GML_M_CHUNK_NUM];
static bool mregistry[GML_M_CHUNK_NUM];

static unsigned char lblock[GML_L_CHUNK_SIZE * GML_L_CHUNK_NUM];
static bool lregistry[GML_L_CHUNK_NUM];

[[nodiscard]] registry_key reserve_chunk(size_t nbytes);
[[nodiscard]] void *access_chunk(registry_key key);
void free_chunk(registry_key key);

size_t get_chunk_size(registry_key key);

} // namespace gml

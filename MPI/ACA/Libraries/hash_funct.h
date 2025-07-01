

#ifndef HASH_FUNCT_H
#define HASH_FUNCT_H

#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "utilities.h"

// FNV Hash constants
#define FNV_OFFSET_BASIS_32 2166136261U
#define FNV_PRIME_32 16777619U
#define FNV_OFFSET_BASIS_64 14695981039346656037ULL
#define FNV_PRIME_64 1099511628211ULL

// xxHash constants
#define XXHASH_PRIME1 2654435761U
#define XXHASH_PRIME2 2246822519U
#define XXHASH_PRIME3 3266489917U
#define XXHASH_PRIME4 668265263U
#define XXHASH_PRIME5 374761393U

// CRC32 polynomial
#define CRC32_POLYNOMIAL 0xEDB88320

// MurmurHash constants
#define MURMUR_SEED 0x5bd1e995
#define MURMUR_R 24

uint32_t fnv1a_hash(const char *str);
uint64_t fnv1a_hash64(const char *str);
uint32_t xxhash32(const char *str, uint32_t seed);
uint32_t crc32_hash(const char *str);
uint32_t murmur2_hash(const char *str, uint32_t seed);
uint64_t djb2_hash(const char *str);
uint64_t add_shift_hash(const char *str);
uint64_t polyhash(const char *str);

//HashPessime
uint64_t xor_h(const char *str);
uint64_t better_xor(const char *str);

#endif

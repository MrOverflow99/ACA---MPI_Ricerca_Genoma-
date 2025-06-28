#include "hash_funct.h"

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// FNV-1a Hash Function (32-bit)
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

uint32_t fnv1a_hash(const char *str) {
    uint32_t hash = FNV_OFFSET_BASIS_32;
    const unsigned char *data = (const unsigned char *)str;

    while (*data) {
        hash ^= *data++;
        hash *= FNV_PRIME_32;
    }

    return hash;
}

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// FNV-1a Hash Function (64-bit)
// 64-bit version for better hash distribution on larger datasets
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

uint64_t fnv1a_hash64(const char *str) {
    uint64_t hash = FNV_OFFSET_BASIS_64;
    const unsigned char *data = (const unsigned char *)str;

    while (*data) {
        hash ^= *data++;
        hash *= FNV_PRIME_64;
    }

    return hash;
}

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// xxHash32 - Extremely fast hash function
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

static uint32_t xxh32_rotl(uint32_t x, int r) {
    return (x << r) | (x >> (32 - r));
}

uint32_t xxhash32(const char *str, uint32_t seed) {
    const unsigned char *data = (const unsigned char *)str;
    size_t len = strlen(str);
    uint32_t h32;

    if (len >= 16) {
        const unsigned char *limit = data + len - 16;
        uint32_t v1 = seed + XXHASH_PRIME1 + XXHASH_PRIME2;
        uint32_t v2 = seed + XXHASH_PRIME2;
        uint32_t v3 = seed + 0;
        uint32_t v4 = seed - XXHASH_PRIME1;

        do {
            v1 = xxh32_rotl(v1 + (*(uint32_t*)data) * XXHASH_PRIME2, 13) * XXHASH_PRIME1;
            data += 4;
            v2 = xxh32_rotl(v2 + (*(uint32_t*)data) * XXHASH_PRIME2, 13) * XXHASH_PRIME1;
            data += 4;
            v3 = xxh32_rotl(v3 + (*(uint32_t*)data) * XXHASH_PRIME2, 13) * XXHASH_PRIME1;
            data += 4;
            v4 = xxh32_rotl(v4 + (*(uint32_t*)data) * XXHASH_PRIME2, 13) * XXHASH_PRIME1;
            data += 4;
        } while (data <= limit);

        h32 = xxh32_rotl(v1, 1) + xxh32_rotl(v2, 7) + xxh32_rotl(v3, 12) + xxh32_rotl(v4, 18);
    } else {
        h32 = seed + XXHASH_PRIME5;
    }

    h32 += (uint32_t)len;

    while (data + 4 <= (unsigned char*)str + len) {
        h32 += (*(uint32_t*)data) * XXHASH_PRIME3;
        h32 = xxh32_rotl(h32, 17) * XXHASH_PRIME4;
        data += 4;
    }

    while (data < (unsigned char*)str + len) {
        h32 += (*data) * XXHASH_PRIME5;
        h32 = xxh32_rotl(h32, 11) * XXHASH_PRIME1;
        data++;
    }

    h32 ^= h32 >> 15;
    h32 *= XXHASH_PRIME2;
    h32 ^= h32 >> 13;
    h32 *= XXHASH_PRIME3;
    h32 ^= h32 >> 16;

    return h32;
}

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// CRC32 Hash Function
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

static uint32_t crc32_table[256];
static int crc32_table_computed = 0;

static void make_crc32_table(void) {
    uint32_t c;
    int n, k;

    for (n = 0; n < 256; n++) {
        c = (uint32_t)n;
        for (k = 0; k < 8; k++) {
            if (c & 1)
                c = CRC32_POLYNOMIAL ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc32_table[n] = c;
    }
    crc32_table_computed = 1;
}

uint32_t crc32_hash(const char *str) {
    uint32_t crc = 0xFFFFFFFF;
    const unsigned char *data = (const unsigned char *)str;

    if (!crc32_table_computed)
        make_crc32_table();

    while (*data) {
        crc = crc32_table[(crc ^ *data++) & 0xFF] ^ (crc >> 8);
    }

    return crc ^ 0xFFFFFFFF;
}

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// MurmurHash2 Function
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

uint32_t murmur2_hash(const char *str, uint32_t seed) {
    const unsigned char *data = (const unsigned char *)str;
    int len = strlen(str);
    const uint32_t m = MURMUR_SEED;
    const int r = MURMUR_R;

    uint32_t h = seed ^ len;

    while (len >= 4) {
        uint32_t k = *(uint32_t*)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len) {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
                h *= m;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

uint64_t add_shift_hash(const char *str) {
    uint64_t hash = 0;
    while (*str) {
        hash += (unsigned char)(*str);
        hash = (hash << 3) | (hash >> (64 - 3));  // Rotate left by 3 bits
        str++;
    }
    return hash % 4096; // Arbitrario per aumentare un po' l'entropia
}

uint64_t djb2_hash(const char *str) {
    uint64_t hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) ^ (unsigned char)(*str); // hash * 33 ^ c
        str++;
    }
    return hash % 8192; // Riduciamo la dimensione per aumentare collisioni
}

uint64_t polyhash(const char *str){
	const uint64_t p = 31;
	const uint64_t m = 1000000009;
	uint64_t hash = 0;
	uint64_t p_pow = 1;

	while(*str){
		hash = (hash + (*str) * p_pow) % m;
		p_pow = (p_pow * p) % m;
		str++;
	}

	return hash;
}



////////////////////////////////////////////////////////////////
/////Funzioni hash pensate apposta per essere pessime///////////
////////////////////////////////////////////////////////////////


uint64_t xor_h(const char *str){
	uint64_t hash = 0;
	while (*str){
		hash ^= *str++;
		return hash % 128;
	}
}

uint64_t better_xor(const char *str){
	uint64_t hash = 0;
	while (*str){
			hash ^= (uint64_t)(*str);
			hash = (hash << 5) | (hash >> (64-5));
			str++;
		}
		return hash % 2048;
}

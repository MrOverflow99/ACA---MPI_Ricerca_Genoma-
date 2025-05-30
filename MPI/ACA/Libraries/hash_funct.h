//
// Created by michele and marco on 5/28/25.
//

#ifndef HASH_FUNCT_H
#define HASH_FUNCT_H

#include "hash_funct.h"
#include <string.h>
#include <math.h>
#include <limits.h>

long long int FNV_hash(char *pattern);
long long int Murmur_hash(char *pattern);
long long int CRC32_hash(char *pattern);
long long int xxHash(char *pattern);

#endif //HASH_FUNCT_H
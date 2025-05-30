//
// Created by michele and marco on 5/28/25.
//

#include "hash_funct.h"
#include <stdint.h>
#include <string.h>

/*  Numeri primi per xxHash  */
#define PRIME1 11400714785074694791ULL
#define PRIME2 14029467366897019727ULL
#define PRIME3  1609587929392839161ULL
#define PRIME4  9650029242287828579ULL
#define PRIME5  2870177450012600261ULL

  long long int FNV_hash(char *pattern){
  long long hash = 14695981039346656037ULL;
  const long long prime = 1099511628211;

  while (*pattern) {
    hash ^= (*pattern++);
    hash *= prime;
  }
  return hash;
}

long long int Murmur_hash(char *pattern){
  return 0;
}

long long int CRC32_hash(char *pattern){
  return 0;
}

long long int xxHash(char *pattern){



  return 0;
}

unsigned long long ROTL64(unsigned long long x, int r) {
    return (x << r) | (x >> (64 - r));
}






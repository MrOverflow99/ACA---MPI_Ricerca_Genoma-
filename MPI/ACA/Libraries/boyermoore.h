//
// Created by miguel on 5/30/25.
//

#ifndef BOYER_MOORE_H
#define BOYER_MOORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "hash_funct.h"
#include <stddef.h>
#include "utilities.h"

#define ALPHABET_SIZE 256
#define max(a, b) ((a) > (b) ? (a) : (b))

// Boyer-Moore algorithm functions
void compute_bad_char_table(const char *pattern, int patlen, int bad_char[ALPHABET_SIZE]);
void compute_good_suffix_table(const char *pattern, int patlen, int *good_suffix);
long long int boyer_moore_search(const char *text, const char *pattern, size_t textlen, size_t patlen);

// Boyer-Moore with hash verification functions
long long int boyer_moore_fnv1a(const char *text, const char *pattern, size_t textlen, size_t patlen);
long long int boyer_moore_xxhash32(const char *text, const char *pattern, size_t textlen, size_t patlen);
long long int boyer_moore_crc32(const char *text, const char *pattern, size_t textlen, size_t patlen);
long long int boyer_moore_murmur2(const char *text, const char *pattern, size_t textlen, size_t patlen);

#endif
#include "boyermoore.h"

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// Compute the bad character table for Boyer-Moore algorithm
// This table stores the rightmost occurrence of each character
// in the pattern, used for skipping characters during search
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

// Wrapper for murmur2 hash to match signature
static uint32_t murmur2_hash_wrapper(const char *str) {
    return murmur2_hash(str, 0x5bd1e995);
}

void compute_bad_char_table(const char *pattern, int patlen, int bad_char[ALPHABET_SIZE]) {
    int i;

    // Initialize all occurrences as -1
    for (i = 0; i < ALPHABET_SIZE; i++)
        bad_char[i] = -1;

    // Fill the actual value of last occurrence of a character
    for (i = 0; i < patlen; i++)
        bad_char[(int)pattern[i]] = i;
}

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// Compute the good suffix table for Boyer-Moore algorithm
// This preprocessing helps in skipping characters when a mismatch
// occurs but a suffix of the pattern matches
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void compute_good_suffix_table(const char *pattern, int patlen, int *good_suffix) {
    int i, j;
    int *border_pos = (int*)malloc(sizeof(int) * (patlen + 1));
    null_check(border_pos);

    // Initialize good suffix table
    for (i = 0; i <= patlen; i++)
        good_suffix[i] = patlen;

    // Compute border positions
    i = patlen;
    j = patlen + 1;
    border_pos[i] = j;

    while (i > 0) {
        while (j <= patlen && pattern[i - 1] != pattern[j - 1]) {
            if (good_suffix[j] == patlen)
                good_suffix[j] = j - i;
            j = border_pos[j];
        }
        i--;
        j--;
        border_pos[i] = j;
    }

    // Compute good suffix values
    j = border_pos[0];
    for (i = 0; i <= patlen; i++) {
        if (good_suffix[i] == patlen)
            good_suffix[i] = j;
        if (i == j)
            j = border_pos[j];
    }

    free(border_pos);
}

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// Standard Boyer-Moore string search algorithm
// Uses bad character and good suffix heuristics for efficient
// pattern matching with potential for skipping many characters
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

long long int boyer_moore_search(const char *text, const char *pattern, size_t textlen, size_t patlen) {
    int bad_char[ALPHABET_SIZE];
    int *good_suffix = (int*)malloc(sizeof(int) * (patlen + 1));
    null_check(good_suffix);

    long long int occurrences = 0;
    int shift = 0;

    // Preprocessing
    compute_bad_char_table(pattern, patlen, bad_char);
    compute_good_suffix_table(pattern, patlen, good_suffix);

    // Searching
    while (shift <= (int)(textlen - patlen)) {
        int j = patlen - 1;

        // Keep reducing index j while characters of pattern and text match
        while (j >= 0 && pattern[j] == text[shift + j])
            j--;

        // If pattern is present at current shift
        if (j < 0) {
            occurrences++;
            // Shift pattern to align with next character in text
            shift += good_suffix[0];
        } else {
            // Shift pattern based on bad character and good suffix heuristics
            shift += max(good_suffix[j + 1], j - bad_char[text[shift + j]]);
        }
    }

    free(good_suffix);
    return occurrences;
}

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// This macro creates Boyer-Moore variants with hash verification
// The hash is computed for pattern and text segments, and only
// when hashes match is the full string comparison performed
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

#define BOYER_MOORE_WITH_HASH(hashfunc, funcname) \
long long int boyer_moore_##funcname(const char *text, const char *pattern, size_t textlen, size_t patlen) { \
    int bad_char[ALPHABET_SIZE]; \
    int *good_suffix = (int*)malloc(sizeof(int) * (patlen + 1)); \
    null_check(good_suffix); \
    \
    long long int occurrences = 0; \
    int shift = 0; \
    \
    /* Preprocessing */ \
    compute_bad_char_table(pattern, patlen, bad_char); \
    compute_good_suffix_table(pattern, patlen, good_suffix); \
    \
    /* Compute pattern hash */ \
    uint32_t pattern_hash = hashfunc(pattern); \
    \
    /* Searching with hash verification */ \
    while (shift <= (int)(textlen - patlen)) { \
        /* Extract substring and compute its hash */ \
        char *substring = (char*)malloc(sizeof(char) * (patlen + 1)); \
        null_check(substring); \
        strncpy(substring, text + shift, patlen); \
        substring[patlen] = '\0'; \
        \
        uint32_t text_hash = hashfunc(substring); \
        \
        /* If hashes match, perform full string comparison */ \
        if (pattern_hash == text_hash) { \
            if (strncmp(pattern, text + shift, patlen) == 0) { \
                occurrences++; \
            } \
        } \
        \
        free(substring); \
        \
        /* Use Boyer-Moore skip logic */ \
        int j = patlen - 1; \
        while (j >= 0 && pattern[j] == text[shift + j]) \
            j--; \
        \
        if (j < 0) { \
            shift += good_suffix[0]; \
        } else { \
            shift += max(good_suffix[j + 1], j - bad_char[(int)text[shift + j]]); \
        } \
    } \
    \
    free(good_suffix); \
    return occurrences; \
}

// Generate Boyer-Moore variants with different hash functions
BOYER_MOORE_WITH_HASH(fnv1a_hash, fnv1a)
BOYER_MOORE_WITH_HASH(crc32_hash, crc32)
BOYER_MOORE_WITH_HASH(murmur2_hash_wrapper, murmur2)



// Special case for xxhash32 which needs seed parameter
long long int boyer_moore_xxhash32(const char *text, const char *pattern, size_t textlen, size_t patlen) {
    int bad_char[ALPHABET_SIZE];
    int *good_suffix = (int*)malloc(sizeof(int) * (patlen + 1));
    null_check(good_suffix);
    
    long long int occurrences = 0;
    int shift = 0;
    
    // Preprocessing
    compute_bad_char_table(pattern, patlen, bad_char);
    compute_good_suffix_table(pattern, patlen, good_suffix);
    
    // Compute pattern hash
    uint32_t pattern_hash = xxhash32(pattern, 0);
    
    // Searching with hash verification
    while (shift <= (int)(textlen - patlen)) {
        // Extract substring and compute its hash
        char *substring = (char*)malloc(sizeof(char) * (patlen + 1));
        null_check(substring);
        strncpy(substring, text + shift, patlen);
        substring[patlen] = '\0';
        
        uint32_t text_hash = xxhash32(substring, 0);
        
        // If hashes match, perform full string comparison
        if (pattern_hash == text_hash) {
            if (strncmp(pattern, text + shift, patlen) == 0) {
                occurrences++;
            }
        }
        
        free(substring);
        
        // Use Boyer-Moore skip logic
        int j = patlen - 1;
        while (j >= 0 && pattern[j] == text[shift + j])
            j--;
        
        if (j < 0) {
            shift += good_suffix[0];
        } else {
            shift += max(good_suffix[j + 1], j - bad_char[(int)text[shift + j]]);
        }
    }
    
    free(good_suffix);
    return occurrences;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include "Libraries/utilities.h"

#define ALPHABET_SIZE 256
#define max(a, b) ((a) > (b) ? (a) : (b))

///////////////////////////////Prototypes\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void compute_bad_char_table(const char *pattern, int patlen, int bad_char[ALPHABET_SIZE]);
void compute_good_suffix_table(const char *pattern, int patlen, int *good_suffix);
long long int boyer_moore_search(const char *text, const char *pattern, size_t textlen, size_t patlen);

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// Compute the bad character table for Boyer-Moore algorithm
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

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
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

long long int boyer_moore_search(const char *text, const char *pattern, size_t textlen, size_t patlen) {
    int bad_char[ALPHABET_SIZE];
    int *good_suffix = (int*)malloc(sizeof(int) * (patlen + 1));
    null_check(good_suffix);

    long long int occurrences = 0;
    size_t shift = 0;

    // Preprocessing
    compute_bad_char_table(pattern, patlen, bad_char);
    compute_good_suffix_table(pattern, patlen, good_suffix);

    // Searching
    while (shift <= (textlen - patlen)) {
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
            shift += max(good_suffix[j + 1], j - bad_char[(int)text[shift + j]]);
        }
    }

    free(good_suffix);
    return occurrences;
}

int main(int argc, char const *argv[])
{
    clock_t begin = clock(); // Tempo inizio totale

    size_t txtlen;
    size_t patlen;

    if (argc != 3) {
        printf("Usage: %s <text_file> <pattern_file>\n", argv[0]);
        printf("Error: Please provide text file and pattern file\n");
        exit(1);
    }

    // Inizio tempo lettura file
    clock_t tempo_begin = clock();

    long long int occurrences = 0;
    char *pattern = readFile((char*)argv[2], &patlen);
    char *txt = readFile((char*)argv[1], &txtlen);

    // Fine tempo lettura file
    clock_t tempo_end = clock();
    double tempo_lettura = (double)(tempo_end - tempo_begin) / CLOCKS_PER_SEC;

    null_check(txt);
    null_check(pattern);

    printf("\nIl tempo per lettura file e': %lf secondi!\n", tempo_lettura);
    printf("Using Boyer-Moore algorithm with hash functions (Serial Version)\n");
    printf("Text length: %zu, Pattern length: %zu\n", txtlen, patlen);
    printf("Pattern: %s\n", pattern);

    occurrences = boyer_moore_search(txt, pattern, txtlen, patlen);
    printf("Total occurrences found: %lld\n", occurrences);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time required: %lf seconds\n", time_spent);
    printf("Algorithm: Boyer-Moore (Serial Implementation)\n");

    free(txt);
    free(pattern);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <time.h>
#include <stddef.h>
#include "./Libraries/utilities.h"
#include "./Libraries/hash_funct.h"
#include "./Libraries/boyermoore.h"

int main(int argc, char *argv[])
{
    int rank;
    int size;
    int isActive = 0;
    int executors;
    char *txt;
    char *pattern;
    char *chunk;
    size_t txtlen;
    size_t patlen;
    size_t chunklen;
    size_t offset = 0;
    long long int occurrences = 0;
    long long int total = 0;
    long long int collisions = 0;
    long long int total_collisions = 0;
    int choice = 0;

    // MPI Initialization
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int flag[size];

    if (rank == 0) {
        txt = readFile(argv[1], &txtlen);
        pattern = readFile(argv[2], &patlen);
        null_check(txt);
        null_check(pattern);
        executors = who_is_active(flag, txtlen, patlen, size);

        printf("Boyer-Moore algorithm with new hash functions\n");
        printf("Text length: %zu\n", txtlen);
        printf("Pattern: %s, Pattern length: %zu\n\n", pattern, patlen);

        printf("Select hash function:\n");
        printf("1. Standard Boyer-Moore (no hash)\n");
        printf("2. FNV-1a\n");
        printf("3. FNV-1a 64Bit\n");
        printf("4. xxHash32\n");
        printf("5. CRC32\n");
        printf("6. MurmurHash2\n");
        printf("7. PolyHash (custom)\n");
        printf("8. DJB2\n");
        printf("9. AddShift-hash\n");
        printf("10. XOR\n");
        printf("11. Better XOR\n");
        printf("Choice: ");
        scanf("%d", &choice);
    }

    clock_t begin = clock();
    MPI_Bcast(&executors, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(flag, 1, MPI_INT, &isActive, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&txtlen, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&patlen, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (isActive) {
        offset = txtlen / executors;
        if (rank == 0) {
            chunk = split_dataset(txt, &chunklen, txtlen, patlen, offset, executors);
            null_check(chunk);
            free(txt);
            for (int i = 1; i < executors; ++i) {
                MPI_Send(pattern, patlen, MPI_CHAR, i, 105, MPI_COMM_WORLD);
            }
        } else {
            pattern = (char *)malloc(sizeof(char) * (patlen + 1));
            null_check(pattern);
            pattern[patlen] = '\0';
            chunk = receive_dataset(offset, txtlen, patlen, &chunklen, rank, executors);
            null_check(chunk);
            MPI_Recv(pattern, patlen, MPI_CHAR, 0, 105, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        switch (choice) {
            case 1:
                occurrences = boyer_moore_search(chunk, pattern, chunklen, patlen);
                break;
            case 2:
                occurrences = boyer_moore_fnv1a(chunk, pattern, chunklen, patlen, &collisions);
                break;
            case 3:
                occurrences = boyer_moore_fnv1a64(chunk, pattern, chunklen, patlen, &collisions);
                break;
            case 4:
                occurrences = boyer_moore_xxhash32(chunk, pattern, chunklen, patlen, &collisions);
                break;
            case 5:
                occurrences = boyer_moore_crc32(chunk, pattern, chunklen, patlen, &collisions);
                break;
            case 6:
                occurrences = boyer_moore_murmur2(chunk, pattern, chunklen, patlen, &collisions);
                break;
            case 7:
                occurrences = boyer_moore_poly(chunk, pattern, chunklen, patlen, &collisions);
                break;
            case 8:
                occurrences = boyer_moore_djb2(chunk, pattern, chunklen, patlen, &collisions);
                break;
            case 9:
                occurrences = boyer_moore_addshift(chunk, pattern, chunklen, patlen, &collisions);
                break;
            case 10:
                occurrences = boyer_moore_xor(chunk, pattern, chunklen, patlen, &collisions);
                break;
            case 11:
                occurrences = boyer_moore_better(chunk, pattern, chunklen, patlen, &collisions);
                break;
            default:
                if (rank == 0) fprintf(stderr, "Invalid choice.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
        }

        free(pattern);
        free(chunk);
    }

    printf("OCCURRENCES rank %d: %lld, COLLISIONS: %lld\n", rank, occurrences, collisions);

    MPI_Reduce(&occurrences, &total, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&collisions, &total_collisions, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    clock_t end = clock();

    if (rank == 0) {
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Total occurrences: %lld\n", total);
        printf("Total hash collisions: %lld\n", total_collisions);
        printf("Time required: %lf seconds\n", time_spent);
        printf("Program executed by %d cores over %d\n", executors, size);

        if (total_collisions > 0) {
            double collision_rate = (double)total_collisions / (double)(total + total_collisions) * 100.0;
            printf("Collision rate: %.4f%%\n", collision_rate);
        }
    }

    MPI_Finalize();
    return 0;
}

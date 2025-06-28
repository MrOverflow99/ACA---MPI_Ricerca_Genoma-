#include "utilities.h"
#include <bsd/string.h>


void null_check(void *ptr){
	if (!ptr)
	{
		printf("Internal error. Please, try again\n");
		exit(1);
	}
}

size_t who_is_active(int *flag, size_t txtlen, size_t patlen, int cores){

	size_t actives = txtlen/patlen;
	if (actives > cores)
	{
		actives = cores;
	}

	for (int i = 0; i < cores; ++i)
	{
		if (i < actives)
		{
			flag[i] = 1;
		} else flag[i] = 0;
	}

	return actives;
}


char *readFile(char *filename, size_t *len) {

    FILE *f = fopen(filename, "r");
    null_check(f);
    fseeko(f, 0, SEEK_END);
    *len = ftello(f);
    rewind(f);
    char *txt = (char *)malloc(*len + 1);
    //null_check(txt);

    if (fread(txt, 1, *len, f) != *len) {
        fprintf(stderr, "Input reading error\n");
        free(txt);
        fclose(f);
        return NULL;
    }

    txt[*len] = '\0';
    fclose(f);
    return txt;
}

char *split_dataset(char *txt, size_t *chunklen, size_t txtlen, size_t patlen, size_t offset, int executors){

	char *chunk = (char *)malloc(sizeof(char) * (offset +1));
	null_check(chunk);
	strlcpy(chunk, txt, offset+1);
	chunk[offset] = '\0';
	*chunklen = offset;

		for (int i = 1; i < executors; ++i)
		{
			if (i <= executors-2)
			{
					MPI_Send(txt+(offset*i)-patlen+1, offset + patlen-1, MPI_CHAR, i, 100, MPI_COMM_WORLD);
			} else {
					MPI_Send(txt+(offset*i)-patlen+1, txtlen - offset*i + patlen -1, MPI_CHAR, i, 101, MPI_COMM_WORLD);
			}
		}
	return chunk;
}

char *receive_dataset(size_t offset, size_t txtlen, size_t patlen, size_t *chunklen, int rank, int executors) {
    char *chunk = NULL;  // Corretto: dichiarato come puntatore

    if (rank < executors-1) {
        *chunklen = offset + patlen - 1;
        chunk = (char *)malloc(sizeof(char) * (*chunklen + 1));
        null_check(chunk);
        chunk[*chunklen] = '\0';
        MPI_Recv(chunk, *chunklen, MPI_CHAR, 0, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        return chunk;
    }

    if (rank == executors-1) {
        *chunklen = txtlen - rank * offset + patlen - 1;
        chunk = (char *)malloc(sizeof(char) * (*chunklen + 1));
        null_check(chunk);
        chunk[*chunklen] = '\0';
        MPI_Recv(chunk, *chunklen, MPI_CHAR, 0, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        return chunk;
    }

    return NULL;
}

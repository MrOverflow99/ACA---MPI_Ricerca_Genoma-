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


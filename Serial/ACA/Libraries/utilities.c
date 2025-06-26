#include "utilities.h"
#include <bsd/string.h>
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//Check if a pointer is null. This function has been created to avoid
//to repeat many times the check of the pointer inside the code.
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void null_check(void *ptr){
	if (!ptr)
	{
		printf("Internal error. Please, try again\n");
		exit(1);
	}
}

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//Check how many cores can be active.
//The splitting of the text is done dividing the length of the text by
//the length of the pattern. If this division is greater than the
//available number of cores, the maximum number of available cores is
//used (active cores); else the quotient of the previous division is
//the number of active cores.
//The vector flag, is like a bitmap used to notify to the cores who
//can be active or not.
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

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

///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//This function computes the length of the file and read it.
//I used ftello because the return value is of type off_t (64 bit) ->
//->it's possible to read file of size > 2GB.
//fseek and ftell are able to handle 32 bits values (at most 2GB)
///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

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


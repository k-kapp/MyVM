#include "../include/scanner.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv [])
{
	if (argc != 2)
	{
		printf("usage: ./main <source filename>\n");
		return EXIT_FAILURE;
	}

    get_text(argv[1]);

    execute_program();

    return EXIT_SUCCESS;
}

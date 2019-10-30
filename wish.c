#include <stdio.h>
#include <stdlib.h>

int main()
{
	char * line = NULL;
	size_t input_size = 0;

	while(1)
	{
		printf("wish> ");

		// Use getline function to read the input from the user, it is a 
		// blocking syscall
		int res = getline(&line, &input_size, stdin);
		
		// End of File
		if( res == -1)
			exit(0);

		if( res == 1)
			// If the input string only contains '\n' char, this implies
			// an empty string, which might just return the prompt to the user
			continue;
		
	}
}
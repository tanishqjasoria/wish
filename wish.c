#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int string_break(char * line, char * tokens[])
{
	int i = 0;
	const char delimiter = ' ';
	// Remove the trailing '\n', if any
	strtok(line, "\n");
	while(1)
		{
			// Break the user input into tokens using a fixed delimiter
			tokens[i] = strsep(&line, &delimiter);
			if(tokens[i] == NULL)
				break;
			i++;
		}
	return i;
}

int main()
{
	char * line = NULL;
	size_t input_size = 0;
	char * input_tokens[10];

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
		else
		{
			int input_len = string_break(line, input_tokens);
		}
	}
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char * path[10];



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

void print_tokens(char * tokens[])
{
  int i = 0;
  while(1)
  {
    if(tokens[i] == NULL)
      break;
    else
    {
      printf("%s ", tokens[i]);
    }
    i++;
  }
}

void update_path(char * tokens[])
{
  int i = 0;
  while(1)
  {
    if(tokens[i] == NULL)
    {
      path[i] = tokens[i];
      break;
    }
    else
    {
      path[i] = tokens[i];
    }
    i++;
  }
}

void change_directory(char * token[])
{
  if(chdir(token[0]) == -1)
  {
    exit(1);
  }
}

int check_redirection(char * tokens[])
{
  int  i = 0;

  while(1)
  {
    if(tokens[i] == NULL)
      break;
    if(strcmp(tokens[i], ">") == 0)
      return i;
    i++;
  }
  return -1;
}




int main()
{
  char * line = NULL;
  size_t input_size = 0;
  char * input_tokens[10];

  int redirection;
  char * redirection_file;

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

      if(strcmp(input_tokens[0], "path") == 0)
      {
        //Code for path command
        update_path(input_tokens + 1);
        print_tokens(path);
      }

      if(strcmp(input_tokens[0], "cd") == 0)
      {
        //Code for cd command
        if(input_len > 2)
        {
          exit(1);
        }
        change_directory(input_tokens + 1);
      }

      if(strcmp(input_tokens[0], "exit") == 0)
      {
        //Code for exit command
        exit(0);
      } 

      int redirection_index = check_redirection(input_tokens);
      if(redirection_index == -1)
      {
        redirection = 0;
      }
      else if (input_len > redirection_index + 2)
      {
        exit(1);
      }
      else
      {
        redirection = 1;
        redirection_file = input_tokens[redirection_index + 1];
        input_tokens[redirection_index] = NULL;
      }
      
      
    }
  }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


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
    {
      printf("\n");
      break;
    }
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
      path[i] = NULL;
      break;
    }
    else
    {
      path[i] =(char *) malloc(strlen(tokens[i]) + 1);
      strcpy(path[i], tokens[i]);
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

int check_redirection(char * tokens[], int * in_out)
{
  int  i = 0;

  while(1)
  {
    if(tokens[i] == NULL)
      break;
    if(strcmp(tokens[i], ">") == 0)
    {
      *in_out = 1;
      return i;
    }
    if(strcmp(tokens[i], "<") == 0)
    {
      *in_out = 2;
      return i;
    }
    i++;
  }
  *in_out = 0;
  return -1;
}

int get_path(char * tokens[])
{
  char temp_path[20];
  
  int i = 0;
  while(1)
  {
    if(path[i] == NULL)
      break;
    else
    {
      strcpy(temp_path, path[i]);
      strcat(temp_path, tokens[0]);
      int  ret_access = access(temp_path, X_OK);
      if(ret_access == 0)
      {
        tokens[0] = (char *)malloc(strlen(temp_path) + 1);
        strcpy(tokens[0], temp_path);
        return 0;
      }
    }
    i++;
  }
  return -1;
}




int main()
{
  char * line = NULL;
  size_t input_size = 0;
  char * input_tokens[10];

  int in_out;
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
        continue;
      }

      if(strcmp(input_tokens[0], "cd") == 0)
      {
        //Code for cd command
        if(input_len > 2)
        {
          exit(1);
        }
        change_directory(input_tokens + 1);
        continue;
      }

      if(strcmp(input_tokens[0], "exit") == 0)
      {
        //Code for exit command
        exit(0);
      } 

      int redirection_index = check_redirection(input_tokens, &in_out);
      
      if ((input_len > redirection_index + 2) && (in_out !=0))
      {
        exit(1);
      }
      else
      {
        redirection_file = input_tokens[redirection_index + 1];
        input_tokens[redirection_index] = NULL;
      }


      int ret = get_path(input_tokens);
      if(ret == -1)
        exit(1);


      int rc = fork();
      if(rc < 0)
      {
        exit(1);
      }
      else if(rc == 0)
      {
        // When a child is created using  the fork call, the value
        // returned is 0.
        if(in_out == 1)
        {
          int fd = creat(redirection_file, 0644);
          dup2(fd, STDOUT_FILENO);
          dup2(fd, STDERR_FILENO);
          close(fd);
        }
        else if(in_out == 2)
        {
          int fd = open(redirection_file, O_RDONLY);
          dup2(fd, STDIN_FILENO);
          close(fd);
        }
        int ret_exec = execv(input_tokens[0], input_tokens);
        if(ret_exec == -1)
          exit(1);
      }
      else
      {
        wait(NULL);
      }
      

    }
  }
}
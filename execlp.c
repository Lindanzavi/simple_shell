#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    char *cmd = NULL;
    size_t bufsize = 0;

    while (1)
    {
        printf("$ ");
        ssize_t nbytes = getline(&cmd, &bufsize, stdin);

        if (nbytes == -1)
        {
            if (feof(stdin))
            {
                printf("\n");
                break;
            }
            else
            {
                perror("getline");
                exit(EXIT_FAILURE);
            }
        }

        // Remove newline character at the end
        if (cmd[nbytes - 1] == '\n')
        {
            cmd[nbytes - 1] = '\0';
        }

        // Tokenize the command
        char *token;
        char *args[100];  // Assume max 100 arguments
        int arg_count = 0;

        token = strtok(cmd, " ");
        while (token != NULL)
        {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;  // Last argument should be NULL

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (execvp(args[0], args) == -1)
            {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            wait(NULL);
        }
    }

    free(cmd);
    return 0;
}

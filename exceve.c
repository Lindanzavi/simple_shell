#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    char *cmd;
    size_t bufsize = 32;
    cmd = (char *)malloc(bufsize * sizeof(char));

    if (cmd == NULL)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("$ ");
        getline(&cmd, &bufsize, stdin);

        // Remove newline character at the end
        cmd[strcspn(cmd, "\n")] = 0;

        if (feof(stdin))
        {
            printf("\n");
            break;
        }

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (execlp(cmd, cmd, NULL) == -1)
            {
                printf("Command not found: %s\n", cmd);
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

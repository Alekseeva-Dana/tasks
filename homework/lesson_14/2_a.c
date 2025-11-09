#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipe1[2], pipe2[2];
    
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        /*pr1 - пишет в первый pipe */
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
        execlp("pr1", "pr1", NULL);
        perror("execlp pr1");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        /* pr2 - читает из первого pipe, пишет во второй */
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
        execlp("pr2", "pr2", NULL);
        perror("execlp pr2");
        exit(1);
    }

    pid_t pid3 = fork();
    if (pid3 == 0) {
        /* pr3 - читает из второго pipe*/
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
        execlp("pr3", "pr3", NULL);
        perror("execlp pr3");
        exit(1);
    }

    /*закрываем все pipe*/
    close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
    
    /*ждем все процессы*/
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    
    return 0;
}

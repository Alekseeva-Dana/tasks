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

 
    pid_t inner_pipe = fork();
    if (inner_pipe == 0) {
        int inner_pipefd[2];
        if (pipe(inner_pipefd) == -1) {
            perror("inner pipe");
            exit(1);
        }

        pid_t pid1 = fork();
        if (pid1 == 0) {
            
            dup2(inner_pipefd[1], STDOUT_FILENO);
            close(inner_pipefd[0]); close(inner_pipefd[1]);
            close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
            execlp("pr1", "pr1", NULL);
            perror("execlp pr1");
            exit(1);
        }

        pid_t pid2 = fork();
        if (pid2 == 0) {
            /* pr2 - читает из внутреннего pipe, пишет в pipe1*/
            dup2(inner_pipefd[0], STDIN_FILENO);
            dup2(pipe1[1], STDOUT_FILENO);
            close(inner_pipefd[0]); close(inner_pipefd[1]);
            close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
            execlp("pr2", "pr2", NULL);
            perror("execlp pr2");
            exit(1);
        }

        close(inner_pipefd[0]); close(inner_pipefd[1]);
        close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
        
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        
        /*после pr1|pr2 выполняем pr3 и пишем в pipe1*/
        pid_t pid3 = fork();
        if (pid3 == 0) {
            dup2(pipe1[1], STDOUT_FILENO);
            close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
            execlp("pr3", "pr3", NULL);
            perror("execlp pr3");
            exit(1);
        }
        waitpid(pid3, NULL, 0);
        exit(0);
    }

    /*pr4 - читает из pipe1*/
    pid_t pid4 = fork();
    if (pid4 == 0) {
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
        execlp("pr4", "pr4", NULL);
        perror("execlp pr4");
        exit(1);
    }

    close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
    
    /*ждем все процессы*/
    waitpid(inner_pipe, NULL, 0);
    waitpid(pid4, NULL, 0);
    
    return 0;
}

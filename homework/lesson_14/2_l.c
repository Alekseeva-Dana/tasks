#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    /*первая часть: pr1 < f.dat | pr2 arg2*/
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    int in_fd = open("f.dat", O_RDONLY);
    if (in_fd == -1) {
        perror("open f.dat");
        exit(1);
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        /*pr1 - читает из файла, пишет в pipe*/
        dup2(in_fd, STDIN_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
        close(in_fd);
        close(pipefd[0]); close(pipefd[1]);
        execlp("pr1", "pr1", NULL);
        perror("execlp pr1");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        /*pr2 - читает из pipe*/
        dup2(pipefd[0], STDIN_FILENO);
        close(in_fd);
        close(pipefd[0]); close(pipefd[1]);
        execlp("pr2", "pr2", "arg2", NULL);
        perror("execlp pr2");
        exit(1);
    }

    /*закрываем дескрипторы первой части*/
    close(in_fd);
    close(pipefd[0]); close(pipefd[1]);
    
    /*ждем завершения первой части*/
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    /*вторая часть: pr3 > f.res*/
    int out_fd = open("f.res", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd == -1) {
        perror("open f.res");
        exit(1);
    }

    pid_t pid3 = fork();
    if (pid3 == 0) {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
        execlp("pr3", "pr3", NULL);
        perror("execlp pr3");
        exit(1);
    }
    close(out_fd);
    waitpid(pid3, NULL, 0);
    
    return 0;
}

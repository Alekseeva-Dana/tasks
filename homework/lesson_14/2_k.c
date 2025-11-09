#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    /*первая часть: pr1 arg1 > f.res*/
    int out_fd1 = open("f.res", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd1 == -1) {
        perror("open f.res");
        exit(1);
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(out_fd1, STDOUT_FILENO);
        close(out_fd1);
        execlp("pr1", "pr1", "arg1", NULL);
        perror("execlp pr1");
        exit(1);
    }
    close(out_fd1);
    waitpid(pid1, NULL, 0);  /*ждем завершения pr1*/

    /*вторая часть: pr2 | pr3 >> f.res*/
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    /*открываем файл для дописывания*/
    int out_fd2 = open("f.res", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (out_fd2 == -1) {
        perror("open f.res");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        /*pr2 - пишет в pipe*/
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]); close(pipefd[1]);
        close(out_fd2);
        execlp("pr2", "pr2", NULL);
        perror("execlp pr2");
        exit(1);
    }

    pid_t pid3 = fork();
    if (pid3 == 0) {
        /*pr3 - читает из pipe, дописывает в файл*/
        dup2(pipefd[0], STDIN_FILENO);
        dup2(out_fd2, STDOUT_FILENO);
        close(pipefd[0]); close(pipefd[1]);
        close(out_fd2);
        execlp("pr3", "pr3", NULL);
        perror("execlp pr3");
        exit(1);
    }

    close(pipefd[0]); close(pipefd[1]);
    close(out_fd2);
    
    /*ждем оставшиеся процессы*/
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    
    return 0;
}

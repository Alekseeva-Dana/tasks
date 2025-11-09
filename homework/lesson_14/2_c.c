#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    /*открываем файл для ввода*/
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
        close(pipefd[0]); close(pipefd[1]);
        close(in_fd);
        execlp("pr1", "pr1", "arg11", "arg12", NULL);
        perror("execlp pr1");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        /*pr2 - читает из pipe*/
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]); close(pipefd[1]);
        close(in_fd);
        execlp("pr2", "pr2", "arg21", "arg22", NULL);
        perror("execlp pr2");
        exit(1);
    }

    close(pipefd[0]); close(pipefd[1]);
    close(in_fd);
    
    /*ждем все процессы*/
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    return 0;
}

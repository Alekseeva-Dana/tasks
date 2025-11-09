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

    /*открываем файл для вывода*/
    int out_fd = open("f.res", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd == -1) {
        perror("open f.res");
        exit(1);
    }

    /*pr1 выполняется последовательно*/
    pid_t pid1 = fork();
    if (pid1 == 0) {
        execlp("pr1", "pr1", NULL);
        perror("execlp pr1");
        exit(1);
    }
    waitpid(pid1, NULL, 0);  /*ждем завершения pr1*/

    /* pr2 | pr3 выполняются параллельноm */
    pid_t pid2 = fork();
    if (pid2 == 0) {
        /*pr2 - пишет в pipe*/
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]); close(pipefd[1]);
        close(out_fd);
        execlp("pr2", "pr2", NULL);
        perror("execlp pr2");
        exit(1);
    }

    pid_t pid3 = fork();
    if (pid3 == 0) {
        /*pr3 - читает из pipe, пишет в файл*/
        dup2(pipefd[0], STDIN_FILENO);
        dup2(out_fd, STDOUT_FILENO);
        close(pipefd[0]); close(pipefd[1]);
        close(out_fd);
        execlp("pr3", "pr3", NULL);
        perror("execlp pr3");
        exit(1);
    }

    close(pipefd[0]); close(pipefd[1]);
    close(out_fd);
    
    /*ждем оставшиеся процессы*/
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    /*открываем файлы*/
    int in_fd = open("f.dat", O_RDONLY);
    int out_fd = open("f.res", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (in_fd == -1) {
        perror("open f.dat");
        exit(1);
    }
    if (out_fd == -1) {
        perror("open f.res");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == 0) {
        /*pr1 - читает из файла, пишет в файл */
        dup2(in_fd, STDIN_FILENO);
        dup2(out_fd, STDOUT_FILENO);
        close(in_fd);
        close(out_fd);
        execlp("pr1", "pr1", NULL);
        perror("execlp pr1");
        exit(1);
    }

    close(in_fd);
    close(out_fd);
    
    /*ждем процесс*/
    waitpid(pid, NULL, 0);
    
    return 0;
}

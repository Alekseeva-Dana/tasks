#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s pr1 [pr2 ... prn]\n", argv[0]);
        exit(1);
    }

    signal(SIGCHLD, SIG_IGN);

    int num_commands = argc - 1;
    int num_pipes = num_commands - 1;
    int pipefds[2 * num_pipes];
    pid_t pids[num_commands];

    /*создаем pipe*/
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipefds + i * 2) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    /*запускаем все команды*/
    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(1);
        }

        if (pids[i] == 0) {
            /*если не первая команда - перенаправляем stdin из предыдущего pipe*/
            if (i > 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }
            
            /*если не последняя команда - перенаправляем stdout в следующий pipe*/
            if (i < num_commands - 1) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }
            
            for (int j = 0; j < 2 * num_pipes; j++) {
                close(pipefds[j]);
            }
            
            /*выполняем команду*/
            execlp(argv[i + 1], argv[i + 1], NULL);
            perror("execlp");
            exit(1);
        } else {
            printf("Started %s with PID: %d\n", argv[i + 1], pids[i]);
        }
    }

    for (int i = 0; i < 2 * num_pipes; i++) {
        close(pipefds[i]);
    }

    printf("Background pipeline started with %d processes\n", num_commands);
    /*не ждем завершения процессов - сразу возвращаем управление*/
    
    return 0;
}

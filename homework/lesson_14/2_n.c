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

    pid_t pids[argc - 1];
    
    for (int i = 1; i < argc; i++) {
        pids[i-1] = fork();
        if (pids[i-1] == -1) {
            perror("fork");
            exit(1);
        }

        if (pids[i-1] == 0) {
          
            execlp(argv[i], argv[i], NULL);
            perror("execlp");
            exit(1);
        } else {
           
            printf("Started %s with PID: %d\n", argv[i], pids[i-1]);
        }
    }

    printf("Background job started with %d processes\n", argc - 1);
    /*не ждем завершения процессов - сразу возвращаем управление*/
    
    return 0;
}

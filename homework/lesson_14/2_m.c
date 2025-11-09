#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s pr1 [pr2 ... prn]\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            
            execlp(argv[i], argv[i], NULL);
            perror("execlp");
            exit(1);
        } else {
            /*родитель ждет завершения текущей команды*/
            waitpid(pid, NULL, 0);
        }
    }

    return 0;
}

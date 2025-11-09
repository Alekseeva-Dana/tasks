#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Должно быть больше аргументов\n");
		exit(1);
	}

	pid_t pids[argc-1];

	/*запускаем все команды параллельно*/
	for (int i = 1; i < argc; i++) {
		pid_t pid = fork();
		if (pid == -1) {
			perror("fork");
			exit(1);
		}

		if (pid == 0) {
			/*дочерний процесс*/
			execlp(argv[i], argv[i], NULL);
			perror("execlp");
			exit(1);
		} else {
			/*родительский процесс сохраняет PID*/
			pids[i - 1] = pid;
		}
	}

	/*ожидаем завершения всех дочерних процессов*/
	for (int i = 0; i < argc - 1; i++) {
		waitpid(pids[i], NULL, 0);
	}

	return 0;
}

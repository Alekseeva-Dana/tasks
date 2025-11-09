#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Отсутствуют арументы\n");
		exit(1);
	}

	int num_commands = argc - 1;
	int pipefds[2 * (num_commands - 1)]; 

	/*создаем pipe*/
	for (int i = 0; i < num_commands - 1; i++) {
		if (pipe(pipefds + i*2) == -1) {
			perror("pipe");
			exit(1);
		}
	}

	pid_t pids[num_commands];

	/*запускаем все команды*/
	for (int i = 0; i < num_commands; i++) {
		pids[i] = fork();
		if (pids[i] == -1) {
			perror("fork");
			exit(1);
		}

		if (pids[i] == 0) {
			/*дочерний процесс*/

			/*если не первая команда - перенаправляем stdin из прудыдущего pipe*/
			if (i > 0) {
				dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
			}

			/*если не последняя команда - перенаправляем stdout в следующий pipe*/
			if (i < num_commands - 1) {
				dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
			}

			for (int j = 0; j < 2 * (num_commands - 1); j++) {
				close(pipefds[j]);
			}

			/*выполняем команду*/
			execlp(argv[i + 1], argv[i + 1], NULL);
			perror("execlp");
			exit(1);
		}
	}

	for (int i = 0; i < 2 * (num_commands - 1); i++) {
		close(pipefds[i]);
	}

	/*ожидаем завершения всех дочерних процессов*/
	for (int i = 0; i < num_commands; i++) {
		waitpid(pids[i], NULL, 0);
	}

	return 0;
}

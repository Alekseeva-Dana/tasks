#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Должен быть один аргумент\n");
		exit(1);
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(1);
	}

	if (pid == 0) {
		/*дочерний процесс*/
		dup2(fd, STDIN_FILENO);
		close(fd);
		execlp("cat", "cat", NULL);
		perror("execlp");
		exit(1);
	} else {
		/*родительский процесс*/
		close(fd);
		wait(NULL);
	}

	return 0;
}

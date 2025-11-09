#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	/*проверяем количество аргументов командной строки*/
	if (argc != 3) {
		fprintf(stderr, "Должно быть два аргумента\n");
		exit(1);
	}
	
	/*открываем файл для записи*/
	int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) {
		perror("open");
		exit(1);
	}
	
	/*создаем дочерний процесс*/
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(1);
	}
	
	/*код для дочернего процесса*/
	if (pid == 0) {
		/*перенаправляем стандартный вывод в файл*/
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execlp(argv[1], argv[1], NULL);
		perror("execlp");
		exit(1);
	/*код для родительского процесса*/
	} else {
		close(fd);
		wait(NULL);
	}

	return 0;
}

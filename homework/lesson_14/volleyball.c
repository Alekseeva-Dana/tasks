#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define MAX_PASSES 8

typedef struct {
    char from[15];  
    int pass_number;
} ball_t;

void player_process(const char* name, int read_fd, int write_fd, const char* next_player) {
    ball_t ball;
    int passes = 0;
    
    
    while (passes < MAX_PASSES) {
        if (read(read_fd, &ball, sizeof(ball_t)) > 0) {
            printf("%s получил мяч от %s (передача #%d)\n", name, ball.from, ball.pass_number);
            
            sleep(5);
            
            strcpy(ball.from, name);
            ball.pass_number++;
            printf("%s передает мяч %s\n", name, next_player);
            
            write(write_fd, &ball, sizeof(ball_t));
            passes++;
        }
    }
    
    printf("%s завершил игру\n", name);
    exit(0);
}

int main() {
    
    int fd_pang_ping[2];   /*Pang -> Ping*/
    int fd_ping_pong[2];   /*Ping -> Pong*/  
    int fd_pong_pang[2];   /*Pong -> Pang*/
    
    /*создаем каналы*/
    if (pipe(fd_pang_ping) == -1 || pipe(fd_ping_pong) == -1 || pipe(fd_pong_pang) == -1) {
        perror("pipe");
        exit(1);
    }
    
    /*процесс PANG*/
    if (fork() == 0) {
        close(fd_pang_ping[0]);
        close(fd_ping_pong[0]); close(fd_ping_pong[1]);
        close(fd_pong_pang[1]);
        
        ball_t first_ball = {"Start", 1};  
        printf("Pang начинает игру\n");
        write(fd_pang_ping[1], &first_ball, sizeof(ball_t));
        
        player_process("Pang", fd_pong_pang[0], fd_pang_ping[1], "Ping");
    }
    
    /*процесс PING*/
    if (fork() == 0) {
        close(fd_pang_ping[1]);
        close(fd_ping_pong[0]);
        close(fd_pong_pang[0]); close(fd_pong_pang[1]);
        
        player_process("Ping", fd_pang_ping[0], fd_ping_pong[1], "Pong");
    }
    
    /*процесс PONG*/
    if (fork() == 0) {
        close(fd_pang_ping[0]); close(fd_pang_ping[1]);
        close(fd_ping_pong[1]);
        close(fd_pong_pang[0]);
        
        player_process("Pong", fd_ping_pong[0], fd_pong_pang[1], "Pang");
    }
    
    /*родительский процесс закрывает все каналы*/
    close(fd_pang_ping[0]); close(fd_pang_ping[1]);
    close(fd_ping_pong[0]); close(fd_ping_pong[1]);
    close(fd_pong_pang[0]); close(fd_pong_pang[1]);
    
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }
    
    printf("\nИгра завершена\n");
    return 0;
}

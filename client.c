#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include "protocol.h"

int client_socket;
int running = 1;

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    
    while (running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_received <= 0) {
            printf("Disconnected from server\n");
            running = 0;
            break;
        }
        
        buffer[bytes_received] = '\0';
        printf("%s", buffer);
        fflush(stdout);
    }
    
    return NULL;
}

void signal_handler(int sig) {
    running = 0;
    close(client_socket);
    printf("\nClient terminated\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        printf("Example: %s 127.0.0.1 8888\n", argv[0]);
        return 1;
    }
    
    char *server_ip = argv[1];
    int port = atoi(argv[2]);
    
    struct sockaddr_in server_addr;
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }
    
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }
    
    printf("Connected to autosalon server at %s:%d\n", server_ip, port);
    printf("Available commands:\n");
    printf("  BUY <car_brand>  - Buy a car from the salon\n");
    printf("  SELL <car_brand> - Sell your car to the salon\n");
    printf("  Type 'exit' to quit\n\n");
    
    signal(SIGINT, signal_handler);
    
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    
    char input[BUFFER_SIZE];
    
    while (running) {
        printf("> ");
        fflush(stdout);
        
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        
        /*удаляем символ новой строки*/
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        
        /*проверка формата команды*/
        if (strlen(input) == 0 || strspn(input, " \t") == strlen(input)) {
            continue;
        }
        
        
        if (strncmp(input, BUY_CMD, 3) == 0 || strncmp(input, SELL_CMD, 4) == 0) {
            /*отправляем команду на сервер*/
            strcat(input, "\n");
            send(client_socket, input, strlen(input), 0);
        } else {
            printf("ERROR: Invalid command format. Use BUY <brand> or SELL <brand>\n");
        }
    }
    
    running = 0;
    close(client_socket);
    pthread_join(recv_thread, NULL);
    
    return 0;
}

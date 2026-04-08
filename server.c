#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include "protocol.h"

#define MAX_CLIENTS 1000
#define MAX_CARS 1000

typedef struct {
    char brand[MAX_CAR_BRAND];
} Car;

typedef struct {
    int socket;
    struct sockaddr_in address;
    pthread_t thread_id;
} Client;

Car car_database[MAX_CARS];
int car_count = 0;
Client clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_socket;

void broadcast(const char *message, int exclude_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket != exclude_socket) {
            send(clients[i].socket, message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

int remove_car(const char *brand) {
    pthread_mutex_lock(&db_mutex);
    for (int i = 0; i < car_count; i++) {
        if (strcmp(car_database[i].brand, brand) == 0) {
            for (int j = i; j < car_count - 1; j++) {
                car_database[j] = car_database[j + 1];
            }
            car_count--;
            pthread_mutex_unlock(&db_mutex);
            return 1; /*успешно удалено*/
        }
    }
    pthread_mutex_unlock(&db_mutex);
    return 0; /*не найдено*/
}

void add_car(const char *brand) {
    pthread_mutex_lock(&db_mutex);
    if (car_count < MAX_CARS) {
        strncpy(car_database[car_count].brand, brand, MAX_CAR_BRAND - 1);
        car_database[car_count].brand[MAX_CAR_BRAND - 1] = '\0';
        car_count++;
    }
    pthread_mutex_unlock(&db_mutex);
}

void *handle_client(void *arg) {
    Client *client = (Client *)arg;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    printf("Client connected: %s:%d\n", 
           inet_ntoa(client->address.sin_addr), 
           ntohs(client->address.sin_port));

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client->socket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_received <= 0) {
            printf("Client disconnected: %s:%d\n", 
                   inet_ntoa(client->address.sin_addr), 
                   ntohs(client->address.sin_port));
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Received from client: %s\n", buffer);

        /*обработка команды buy*/
        if (strncmp(buffer, BUY_CMD, 3) == 0) {
            char brand[MAX_CAR_BRAND];
            if (sscanf(buffer + 4, "%49s", brand) == 1) {
                if (remove_car(brand)) {
                    /*уведомление о продаже*/
                    snprintf(response, BUFFER_SIZE, "%s Car '%s' has been sold. Congratulations!\n", 
                             NOTIFY_PREFIX, brand);
                    send(client->socket, response, strlen(response), 0);
                    
                    /*рассылка всем*/
                    snprintf(response, BUFFER_SIZE, "%s Car '%s' was purchased from the salon.\n", 
                             INFO_PREFIX, brand);
                    broadcast(response, client->socket);
                } else {
                    snprintf(response, BUFFER_SIZE, "%s Car '%s' is not available.\n", 
                             ERROR_PREFIX, brand);
                    send(client->socket, response, strlen(response), 0);
                }
            } else {
                snprintf(response, BUFFER_SIZE, "%s Invalid BUY command format. Use: BUY <brand>\n", 
                         ERROR_PREFIX);
                send(client->socket, response, strlen(response), 0);
            }
        }
        /* обработка команды sell*/
        else if (strncmp(buffer, SELL_CMD, 4) == 0) {
            char brand[MAX_CAR_BRAND];
            if (sscanf(buffer + 5, "%49s", brand) == 1) {
                add_car(brand);
                
                snprintf(response, BUFFER_SIZE, "%s Car '%s' added to salon. Thank you!\n", 
                         SUCCESS_PREFIX, brand);
                send(client->socket, response, strlen(response), 0);
                
                /*рассылка всем*/
                snprintf(response, BUFFER_SIZE, "%s New car '%s' is available in the salon.\n", 
                         INFO_PREFIX, brand);
                broadcast(response, -1);
            } else {
                snprintf(response, BUFFER_SIZE, "%s Invalid SELL command format. Use: SELL <brand>\n", 
                         ERROR_PREFIX);
                send(client->socket, response, strlen(response), 0);
            }
        }
        else {
            snprintf(response, BUFFER_SIZE, "%s Unknown command. Use BUY <brand> or SELL <brand>\n", 
                     ERROR_PREFIX);
            send(client->socket, response, strlen(response), 0);
        }
    }

    /*удаляем клиента из списка*/
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket == client->socket) {
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    close(client->socket);
    free(client);
    return NULL;
}

void signal_handler(int sig) {
    printf("\nShutting down server...\n");
    
    /*закрываем все клиентские сокеты*/
    for (int i = 0; i < client_count; i++) {
        close(clients[i].socket);
    }
    
    close(server_socket);
    exit(0);
}

int main(int argc, char *argv[]) {
    int port = SERVER_PORT;
    
    if (argc > 1) {
        port = atoi(argv[1]);
    }
    
    struct sockaddr_in server_addr;
    
    /*создание сокета*/
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    
    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        exit(1);
    }
    
    printf("Autosalon server started on port %d\n", port);
    
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_socket = accept(server_socket, 
                                   (struct sockaddr*)&client_addr, 
                                   &client_len);
        
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        pthread_mutex_lock(&clients_mutex);
        if (client_count < MAX_CLIENTS) {
            Client *new_client = malloc(sizeof(Client));
            new_client->socket = client_socket;
            new_client->address = client_addr;
            
            clients[client_count++] = *new_client;
            
            pthread_create(&new_client->thread_id, NULL, handle_client, new_client);
            pthread_detach(new_client->thread_id);
        } else {
            char *msg = "ERROR: Server is full. Try again later.\n";
            send(client_socket, msg, strlen(msg), 0);
            close(client_socket);
        }
        pthread_mutex_unlock(&clients_mutex);
    }
    
    return 0;
}

CC = gcc
CFLAGS = -Wall -Wextra -pthread -Wno-unused-parameter

all: server client

server: server.c protocol.h
	$(CC) $(CFLAGS) server.c -o server

client: client.c protocol.h
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f server client

run_server: server
	./server 8888

run_client: client
	./client 127.0.0.1 8888

test: server client
	@echo "Тестирование автосалона..."
	@echo "Запустите в разных терминалах:"
	@echo "1. make run_server"
	@echo "2. make run_client"
	@echo "3. ./client 127.0.0.1 8888"

help:
	@echo "Доступные команды:"
	@echo "  make all         - собрать сервер и клиент"
	@echo "  make server      - собрать только сервер"
	@echo "  make client      - собрать только клиента"
	@echo "  make clean       - удалить исполняемые файлы"
	@echo "  make run_server  - запустить сервер на порту 8888"
	@echo "  make run_client  - запустить клиента"
	@echo "  make test        - инструкции для тестирования"
	@echo "  make help        - показать эту справку"

.PHONY: all clean run_server run_client test help

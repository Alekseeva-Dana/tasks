<h2 align="center">Описание <a href="https://github.com/Alekseeva-Dana/tasks/tree/main/2sem/Task1" target="_blank">Task1</h2>
Выбранный мной вариант - вариант 5 (автосалон): программа-сервер имитирует автосалон, а программы-клиенты - покупателей, которые продают(SELL), либо покупают(BUY) автомобили
<h3>модуль client.c</h3>
<b>функция приема сообщений:</b>
<pre><code>void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE]; </code></pre>
Ключевая команда в функции - recv - получение данных из сокета(в буфер)
Если соединение разорвано, цикл работы программы останавливается
<b>обработчик сигналов</b>
<pre><code>void signal_handler(int sig) {  // Обработчик сигнала (например, Ctrl+C)
    running = 0;  // Останавливаем программу
    close(client_socket);  // Закрываем сокет
    printf("\nClient terminated\n");
    exit(0);  // Завершаем программу
}</code></pre>
В основной функции(main) подключаемся к серверу через порт(argv[2]), создаем сокет, устанавливаем обработку сигналов, создаем поток для приема сообщений. В основном цикле while(running) читаем ввод и отправляем команды

<h3>модуль server.c</h3>
Описаны необходимые структуры:
<pre><code>
  typedef struct {
    char brand[MAX_CAR_BRAND];  // Марка автомобиля
} Car;

typedef struct {
    int socket;  // Дескриптор сокета клиента
    struct sockaddr_in address;  // Адрес клиента
    pthread_t thread_id;  // Идентификатор потока клиента
} Client;
</code></pre>
Описаны функции рассылки сообщений,
<pre><code>
    void broadcast(const char *message, int exclude_socket) {
    pthread_mutex_lock(&clients_mutex);  // Блокируем доступ к списку клиентов
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket != exclude_socket) {  // Пропускаем исключенного клиента
            send(clients[i].socket, message, strlen(message), 0);  // Отправка сообщения
        }
    }
    pthread_mutex_unlock(&clients_mutex);  // Разблокируем мьютекс
}
</code></pre>
удаления автомобиля,
<pre><code>
    int remove_car(const char *brand) {
    pthread_mutex_lock(&db_mutex);  // Блокируем доступ к базе данных
    for (int i = 0; i < car_count; i++) {
        if (strcmp(car_database[i].brand, brand) == 0) {  // Находим автомобиль
            for (int j = i; j < car_count - 1; j++) {
                car_database[j] = car_database[j + 1];  // Сдвигаем элементы
            }
            car_count--;  // Уменьшаем количество
            pthread_mutex_unlock(&db_mutex);
            return 1;  // Успешное удаление
        }
    }
    pthread_mutex_unlock(&db_mutex);
    return 0;  // Автомобиль не найден
}
</code></pre>
добавления автомобиля, 
<pre><code>
    void add_car(const char *brand) {
    pthread_mutex_lock(&db_mutex);  // Блокируем доступ к базе данных
    if (car_count < MAX_CARS) {  // Проверяем, есть ли место
        strncpy(car_database[car_count].brand, brand, MAX_CAR_BRAND - 1);
        // Копируем марку с ограничением длины
        car_database[car_count].brand[MAX_CAR_BRAND - 1] = '\0';  // Гарантируем нуль-терминатор
        car_count++;  // Увеличиваем количество
    }
    pthread_mutex_unlock(&db_mutex);  // Разблокируем мьютекс
}
</code></pre>
обработчика клиента, обработки команды BUY, обработки команды SELL, удаления клиента из списка,
<pre><code>
        pthread_mutex_lock(&clients_mutex);  // Блокируем доступ к списку
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket == client->socket) {  // Находим клиента
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];  // Сдвигаем элементы
            }
            client_count--;  // Уменьшаем количество
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);  // Разблокируем

    close(client->socket);  // Закрываем сокет клиента
    free(client);  // Освобождаем память
    return NULL;  // Завершение потока
}
</code></pre>
обработчика сигналов сервера
<pre><code>
    void signal_handler(int sig) {
    printf("\nShutting down server...\n");
    
    for (int i = 0; i < client_count; i++) {
        close(clients[i].socket);  // Закрываем все клиентские сокеты
    }
    
    close(server_socket);  // Закрываем сокет сервера
    exit(0);  // Завершаем программу
}
</code></pre>
В основной функции(main) создаем сокет, начинаем слушать соединения, принимаем новое соединение.

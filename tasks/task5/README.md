<h2 align="center">Описание <a href="https://github.com/Alekseeva-Dana/tasks/tree/main/tasks/task5" target="_blank">Task5</h2>
<b>Описание идеи и работы программы:</b>
<b>sigchld_handler</b> - обработка сигнала SIGCHLD, который посылается при завершении дочерних процессов. Выводит информацию о завершенных фоновых процессах
<pre><code>void sigchld_handler(int sig)
{
    (void) sig;
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("\n[Процесс %d завершился с кодом %d]\n", pid, WEXITSTATUS(status));           
        }
        else if (WIFSIGNALED(status))
        {
            printf("\n[Процесс %d завершён сигналом %d]\n", pid, WTERMSIG(status));
        }
        fflush(stdout);
    }
}</code></pre>
<b>child_exec</b> - функция для выполнения команд в дочернем процессе
<pre><code>void child_exec(char** argv, int in_fd, int out_fd, bool redirect_out, char* outfile, words_t* words)
{
    // Перенаправление ввода (stdin)
    if (in_fd != -1)
    {
        if (dup2(in_fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            free_words(words);
            exit(1);
        }
        close(in_fd);
    }
    if (out_fd != -1)  // Перенаправление вывода (stdout)
    {
        if (dup2(out_fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            free_words(words);
            exit(1);
        }
        close(out_fd);
    }
    // Перенаправление вывода в ФАЙЛ
    if (redirect_out) 
    {
        int fdout = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fdout < 0)
        {
            perror("open");
            free_words(words);
            exit(1);
        }
        fflush(stdout);
        if (dup2(fdout, STDOUT_FILENO) == -1) 
        {
            perror("dup2");
            close(fdout);
            free_words(words);
            exit(1);
        }
        close(fdout);
    }
    execvp(argv[0], argv);
    perror("execvp");
    free_words(words);
    exit(1);
}</code></pre>
-перенаправляет ввод/вывод через дескрипторы файлов
-перенаправляет вывод в файл (оператор >)
-заменяет текущий процесс новой прораммой через execvp()
<b>execute_command</b> - управление процессами
<pre><code>void execute_command(words_t* words, bool background, bool redirect_out, char* outfile)
{
    pid_t pid = fork();
    if (pid == 0)  // дочерний процесс
    {
        child_exec(words->data, -1, -1, redirect_out, outfile, words);
    }
    else if (pid > 0)  // родительский процесс
    {
        if (background)  // фоновый режим
        {
            printf("[Запущен фоновый процесс %d]\n", pid);
        }
        else  // интерактивный режим - ждём завершения
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    else
    {
        perror("fork");
    }
}</code></pre>
создает новый процесс fork() и управляет им: 
  -для фоновых процессов не ждет завершения
  -для интерактивных процессов ожидает завершения
<b>read_line</b> - чтение ввода пользователя
<pre><code>errcode_t read_line(char** input)
{
    // ... (динамическое выделение памяти и чтение строки)
    return err;
}</code></pre>
читает строку ввода от пользователя с динамическим увеличением буфера при необходимости
Далее набор функций для работы <b>с памятью для слов</b>
<pre><code>errcode_t reserve_memory(words_t* words, size_t n)
{
    // ... (увеличение ёмкости массива слов при необходимости)
    return err;
}

errcode_t add_word(words_t* words, const char* word)
{
    // ... (добавление нового слова в массив)
    return err;
}

errcode_t copy_word(size_t start, size_t current_symbol, const char* line, words_t* words)
{
    // ... (копирование подстроки как отдельного слова)
    return err;
}

void free_words(words_t* words)
{
    // ... (освобождение всей выделенной памяти)
}</code></pre>
управляет памятью для хранения разобранных слов командной строки
<b>лексический анализ(разбор строки)</b>
<pre><code>bool is_special(char c) 
{ 
    return strchr(SPECIALS, c) != NULL;
}

bool is_double_special(char c1, char c2) 
{
    // ... (проверка на двойные операторы)
    return false;
}

bool is_regular(char c) 
{ 
    return !isspace(c) && !is_special(c) && c != '"'; 
}

errcode_t split_line(const char* line, words_t* words)
{
    // ... (разделение строки на токены с учётом кавычек и спецсимволов)
    return err;
}</code></pre>
разбивает введённую строку на токены (слова, операторы), обрабатывая:
  -специальные символы как отдельные токены
  -двойные операторы (>>, && и т.д.)
  -строки в кавычках как единые токены
  -обычные слова
В основной функции <b>main</b> (главный цикл оболочки):
1.выводим приглашение командной строки
2.читаем команду пользователя
3.разбирает её на токены
4.обрабатываем встроенные команды (cd)
5.определяет тип выполнения (конвейер, фон, перенаправление)
6.выполняем команду
7.очищаем память и повторяем цикл
<b>ОСНОВНЫЕ КОМПОНЕНТЫ ОБОЛОЧКИ:</b>
1.лексер - разбивает строку на токены (split_line, is_special, is_regular)
2.парсер - анализирует структуру команды (конвейеры, перенаправления)
3.исполнитель - создаёт процессы и выполняет команды (execute_command, child_exec)
4.управление памятью - динамическое выделение/освобождение памяти
5.обработка сигналов - управление фоновыми процессами
6.встроенные команды - cd реализована внутри оболочки

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //системные вызовы fork execvp
#include <ctype.h>
#include <errno.h> //коды ошибок системы
#include <fcntl.h> //управление файловыми дескрипторами
#include <signal.h> //обработчики сигналов signal()
#include <stdbool.h> //булевские перем
#include <string.h>
#include <sys/wait.h> //ожидание процессов wait() waitpid()


//ОБЬЯВЛЕНИЕ ГЛОБАЛЬНЫХ КОНСТАНТ
const char* SPECIALS = "><@#%!&$^;:,(){}[]";
const char* DOUBLE_SPECIALS[] = {">>", "<<", "||", "&&", NULL};
const size_t INIT_BUF_SIZE = 128;
const size_t INIT_WORDS_SIZE = 4;


typedef struct 
{
    char** data; //массив из слов 
    size_t n; // количество слов сейчас (сколько занято мест)
    size_t cap; // // сколько вообще слов мб 
} words_t;



typedef enum {ERR_EXIT, ERR_OK, ERR_MEM} errcode_t;


//ОБЬЯВЛЕНИЕ ФУНКЦИЙ
bool is_special(char c);
bool is_double_special(char c1, char c2);
bool is_regular(char c);
void sigchld_handler(int sig);
void free_words(words_t* words);
errcode_t read_line(char** input);
errcode_t reserve_memory(words_t* words, size_t n);
errcode_t add_word(words_t* words, const char* word);
errcode_t copy_word(size_t start, size_t current_symbol, const char* line, words_t* words);
errcode_t split_line(const char* line, words_t* words);






// Обработчик SIGCHLD — вызывается, когда дочерний процесс завершается
//handler для фоновых
void sigchld_handler(int sig) //арг - № сигнала
{
    (void) sig; //чтоб не ругался что не используем
    int status; //битовая маска
    pid_t pid;

    // waitpid(-1, ...) — обрабатывает всех завершившихся потомков
    //если их нет - не ждем
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0 )
    {
        if (WIFEXITED(status)) //если норм завершился
        {
            printf("\n[Процесс %d завершился с кодом %d]\n", pid, WEXITSTATUS(status));           
        }
        else if (WIFSIGNALED(status))  //если убит сигналом
        {
            printf("\n[Процесс %d завершён сигналом %d]\n", pid, WTERMSIG(status));
        }
        fflush(stdout); //принудительно выводим печать
    }
}




//запуск команды в дочернем процессе с потоками ввода/вывода
void child_exec(char** argv, int in_fd, int out_fd, bool redirect_out, char* outfile, words_t* words) 
{
    //Перенаправление ВВОДА (stdin)
    if (in_fd != -1) //то нужно перенаправить ввод
    {
        if (dup2(in_fd, STDIN_FILENO) == -1) //если -1 - ошибка   если = 0 делает копию in_fd как stdin (дескриптор 0) и Закрывает оригинальный in_fd (чтобы не висел открытым)
        {
            perror("dup2");
            free_words(words);
            exit(1);
        } //Для конвейера cmd1 | cmd2 - cmd2 получает вывод cmd1 как свой ввод
        close(in_fd);
    }

    if (out_fd != -1)  //Перенаправление ВЫВОДА (stdout)
    {
        if (dup2(out_fd, STDOUT_FILENO) == -1) //делает out_fd stdout (дескриптор 1)
        {
            perror("dup2");
            free_words(words);
            exit(1);
        } // Для конвейера cmd1 | cmd2 - cmd1 пишет в канал вместо stdout
        close(out_fd);
    }

    
    /*  
        open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644):
        O_WRONLY - только запись
        O_CREAT - создать если нет
        O_TRUNC - очистить файл если существует
        0644 - права: владелец RW, остальные R
        dup2(fdout, STDOUT_FILENO) - перенаправляет stdout в файл

        Для ls > files.txt - вывод ls идет в файл
    */
    //Перенаправление вывода в ФАЙЛ
    if (redirect_out) 
    {
        int fdout = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fdout < 0) //если ошибка открытия
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

    execvp(argv[0], argv); // ЗАМЕНЯЕТ текущий процесс новой программой
    perror("execvp"); //иначе ошибка
    free_words(words);
    exit(1);
}





void execute_command(words_t* words, bool background, bool redirect_out, char* outfile) 
{
    //запуск команды
    pid_t pid = fork();
    if (pid == 0)//успешно - дочерний
    {
        //дочерний процесс
        // Передаём -1 для in_fd/out_fd когда ввод-вывод по-умолчанию
        child_exec(words->data, -1, -1, redirect_out, outfile, words);
        //ребенок завершается -ничего не ждем
    }
    else if (pid > 0) //родительский
    {
        //если запущен фоновый
        if (background) //в фоновом не ждем завершения ребенка
        {
            printf("[Запущен фоновый процесс %d]\n", pid);
        }
        else //ждем завершения ребенка
        {
            int status;
            waitpid(pid, &status, 0); // ждём, если не фоновый
        }
    }
    else
    {
        perror("fork");
    }
}




errcode_t read_line(char** input)
{
    errcode_t err = ERR_OK;
    size_t bufsize = INIT_BUF_SIZE;

    char* buffer = malloc(bufsize);

    if (buffer == NULL)
    {
        perror("malloc error in read_line()");
        err = ERR_MEM;
    }
    else
    {
        size_t len = 0;
        int c;
        while (((c = getchar()) != EOF) && (c != '\n'))
        {
            if ((len+1) >= bufsize) //+1 - +место для /0 да еще и новый элем
            {
                bufsize *= 2;
                buffer = realloc(buffer, bufsize);
                if (buffer == NULL)
                {
                    perror("realloc error in read_line()");
                    err = ERR_MEM;
                    break;
                }
            }
            buffer[len] = (char)c; //записали элем
            len++; //увеличили длину
        }

        if (c == EOF && len == 0)
        {
            free(buffer);
            err = ERR_EXIT; //есди пустая строка
        }
        else
        {
            buffer[len] = '\0';
            *input = buffer;
        }
    }
    return err; //записали строку (передача по ссылке) и вернули ошибку
}




//выделение места под слова в массиве слов (для разбивки)
//арг: глоб массив и размер (сколько уже есть слов)
errcode_t reserve_memory(words_t* words, size_t n)
{
    errcode_t err = ERR_OK;
    //колво слов сейчас +1 новое > чем мы выделили слов?
    if ((n + 1) >= words->cap)
    {
        if (words->cap == 0)
        {
            words->cap = n;
        }
        else
        {
            words->cap *= 2;
        }

        //выделяем место в **data для большего кол-ва слов
        char** buffer = realloc(words->data, words->cap *sizeof(char*));
        if (buffer == NULL)
        {
            perror("realloc error in reserve_memory()");
            err = ERR_MEM;
        }
        else
        {
            words->data = buffer;
        }
    }
    return err;
}




//добавляем выделенное слово в массив (арг: глоб структура и выдел слово)
//words - указатель на структуру
//words->data - массив указателей
errcode_t add_word(words_t* words, const char* word)
{
    //проверка - есть ли память (место) на это новое слово
    errcode_t err = reserve_memory(words, words->n);
    if (err == ERR_OK)
    {
        //Указатель на позицию n (n = текущее количество слов)
        //копируем строку и на последнее место ставим указатель на это новое слово
        words->data[words->n] = strdup(word);
        words->n +=1; //увеличивает счетчик слов (которые записали)
    }

    return err;
}


//считываем слово и внутри через др функцию добавляем в массив новое слово 
//(передаем индекс начала и конца слова)
errcode_t copy_word(size_t start, size_t current_symbol, const char* line, words_t* words) 
{
    errcode_t err = ERR_OK;
    size_t word_len = current_symbol - start;
    char* word = malloc(word_len + 1);
    if (word == NULL)
    {
        perror("malloc error in copy_word()");
        err = ERR_MEM; 
    }
    else
    {
        // копирование части строки
        strncpy(word, line+start, word_len);
        word[word_len] = '\0';

        //добавляем в массив
        err = add_word(words, word);///////////////////
        free(word);
    }
    return err;

}



// является ли символ специальным символом из НАШЕГО СПИСКА В ЗАДАНИИ
bool is_special(char c) 
{ 
    return strchr(SPECIALS, c) != NULL; //ture - false возвращ
}



//если это двойной символ (напр << )  - запишем как 1 полноценный
bool is_double_special(char c1, char c2) 
{
  char pair[3] = {c1, c2, '\0'};
  //и сравниваем с теми двойными символами которые у нас могут быть - иначе ошибка
  for (int i = 0; DOUBLE_SPECIALS[i]; ++i) {
    if (strcmp(pair, DOUBLE_SPECIALS[i]) == 0) {
      return true;
    }
  }
  return false;
}


//символ норм - НЕ пустота и НЕ спецсимвол и НЕ кавычки
bool is_regular(char c) 
{ 
    return !isspace(c) && !is_special(c) && c != '"'; 
}


// Разделение строки на слова
errcode_t split_line(const char* line, words_t* words)
{
    //выделяем место для слов (изменяем и передаем первонач структуру (новую не создаем))
    errcode_t err = reserve_memory(words, INIT_WORDS_SIZE);

    if (err != ERR_OK) return err;

    size_t i = 0, len = strlen(line), start = 0;

    while (i < len && err == ERR_OK)
    {
        //пропуск пробелов //слово или спецсимвол
        while(i < len && isspace(line[i]))
        {
            i++;
        }
        if (i>= len) break;

        //внутри кавычек
        if (line[i] == '"')
        {
            i+=1;
            start = i;
            while ((i < len) && (line[i] != '"'))
            {
                i++;
            }
            //передаем индекс начала и конца слова
            err = copy_word(start, i, line, words);

            if (err == ERR_OK && i<len && line[i] == '"')
            {
                i+=1;
            }
        }

        // Спецсимвол как отдельное слово
        else if (is_special(line[i]))
        {
            char word[3] = {0}; //буфер на 3 символа
            word[0] = line[i];
            word[1] = '\0';
            if (i+1 < len && is_double_special(line[i], line[i+1]))
            {
                word[1] = line[i+1];
                word[2] = '\0';
                i+=1;
            }

            add_word(words, word); //добавляем в массив слово
            i+=1;
        }

        //обычное слово
        else
        {
            start = i;
            while(i<len && is_regular(line[i]))
            {
                i++;
            }
            //добавляем слово
            err = copy_word(start, i, line, words);
        }
        // чтобы execvp корректно распознал конец списка аргументов
        // и не читал память за пределами массива, ставим NULL в конце
        err = reserve_memory(words, words->n);
        if (err == ERR_OK)
        {
            words->data[words->n] = NULL;
        }
    }
    return err;
}




//освобождение памяти всей структуры (динам массива и кол-ва слов)
void free_words(words_t* words)
{
    for (size_t i=0; i<words->n; ++i)
    {
        free(words->data[i]);
    }

    free(words->data);
    words->n = 0;
    words->cap = 0;
}




int main(void)
{
    errcode_t err = ERR_OK; //флаг - нет ошибок (статус выполн опер)

    // для фоновых
    signal(SIGCHLD, sigchld_handler); //посланный сигнал после заверш дочернего
    //все, настроили правила - можем работать


    while(1)
    {
        printf("\033[35m $\033[0m ");  // Зеленый "=>"
        fflush(stdout);
        char *input = NULL;
        err = read_line(&input); //передаем адрес указателя
        //в input - строка (что написали в ввод)

        if ((err == ERR_EXIT) || (err == ERR_MEM)) break; //если пустая строка или ошиб выдел памяти
        else if (err = ERR_OK)
        {
            //ЕСЛИ ВСЕ ОК - ОСНОВНАЯ ОБРАБОТКА ВВЕД КОМАНДЫ

            words_t words = {.data = NULL, .n = 0, .cap = 0};
            err = split_line(input, &words); //арг считанная строк и массив разбивки
            //создали массив из слов и спецсимволов

            if (err == ERR_OK)
            {
                //если пустая строка - пропускаем
                if (words.n == 0)
                {
                    free_words(&words);
                    continue;
                }

                // cd - команда. Реализуем отдельно
                if (strcmp(words.data[0], "cd") == 0)
                {
                    if (words.n < 2)
                    {
                        fprintf(stderr, "cd: отсутствует аргумент\n");
                    }
                    else
                    {
                        //0 при успехе, -1 при ошибке
                        if (chdir(words.data[1]) != 0)
                        {
                            perror("cd");
                        }
                    }
                    //очищаем - выходим на новую итерац цикла
                    free_words(&words);

                    continue;
                }


                //проверка на конвейер (нам такое не надо)
                int pipe_pos = -1;
                for (size_t i = 0;i < words.n; i++)
                {
                    if (strcmp(words.data[i], "|") == 0)
                    {
                        if (pipe_pos != -1)
                        {
                            fprintf(stderr, "Ошибка: поддерживается только один |\n");
                            pipe_pos = -2;
                            break;                            
                        }
                        pipe_pos = i; //запоминает позицию первого вхожд
                    }
                }

                //если больше чем 1 разделитель "|"
                if (pipe_pos == -2)
                {
                    free_words(&words);
                    continue;
                }

                //проверка на фоновый запуск (ищем &)
                bool background = false; //флаг фонового режима
                for (size_t i=0; i<words.n; ++i)
                {
                    if (strcmp(words.data[i], "&") == 0)
                    {
                        //если это НЕ последний символ
                        if (i != (words.n - 1))
                        {
                            fprintf(stderr, "Ошибка: символ '&' должен быть последним в команде\n");
                        }

                        //если & все таки стоит последним в конце
                        else
                        {
                            background = true;
                            free(words.data[i]); //очищаем это слово
                            words.data[i] = NULL;
                            words.n -= 1;
                        }
                    }
                }


                //Проверка на вывод в файл (Перенаправление ">")
                bool redirect_out = false;
                char* outfile = NULL;

                for (size_t i = 0; i < words.n; i++)
                {
                    if (strcmp(words.data[i], ">") == 0)
                    {
                        //если аргументов после > нет имени файла - ошибка
                        if (i+1 >= words.n)
                        {
                            fprintf(stderr, "Ошибка: > без имени файла\n");
                            redirect_out = false;
                            outfile = NULL;
                            break;
                        }

                        redirect_out = true;
                        outfile = words.data[i+1];
                        free(words.data[i]); // Освобождаем ">"
                        words.data[i] = NULL; // и зануляем
                        /*
                        Чтобы execvp видел: ["cmd", "arg", NULL, "file"]
                        Без этого execvp получил бы: ["cmd", "arg", ">", "file"] и сломался
                        */
                        break; //нашли > и все удачно
                    }
                }


                //поиск разделителей для конвейера |
                if (pipe_pos != -1) //если все таки нашли и он один "|" в строке
                {
                    free(words.data[pipe_pos]); // Освобождаем "|"
                    words.data[pipe_pos] = NULL; // и зануляем

                    //разделение команд (строк до и после "|") для конвейера
                    char** argv1 = words.data; //указатель на начало массива слов
                    char** argv2 = &words.data[pipe_pos + 1]; //указатель на середину массива (с того слова которое после "|")

                    int fd[2]; //массив для 2х файл дескрипторов i: 0 - для чтения 1 - для записи
                    //создали канал pipe
                    if (pipe(fd) == -1) // 0 - успех; -1 - ошибка
                    {
                        perror("pipe");
                        free_words(&words);//очищаем память 1й команды и переходим ко 2й
                        continue;
                    }

                    // ПЕРВАЯ КОМАНДА
                    pid_t pid1 = fork();
                    if (pid1 == -1) //ошибка создания
                    {
                        // Дочерний процесс 1 НЕ выполняется здесь сразу!
                        // Он "спит" в очереди планировщика
                        perror("fork для первой команды");
                        close(fd[0]);
                        close(fd[1]);
                        free_words(&words);
                        continue;
                    }
                    else if (pid1 == 0) //успешно - дочерний
                    {
                        /* Дочерний процесс 1: конец pipe для записи */
                        close(fd[0]);
                        child_exec(argv1, -1, fd[1], false, NULL, &words);

                    }

                    //ВТОРАЯ КОМАНДА
                    pid_t pid2 = fork();
                    if (pid2 == -1) //ошибка создания
                    {
                        // Мы зашли СЮДА - в родительский процесс!
                        // Но процессы 1 и 2 МОГУТ быть уже запущены!
                        perror("fork для второй команды");
                        close(fd[0]);
                        close(fd[1]);

                        // Убиваем первый процесс если он запустился
                        if (pid1 > 0) {
                            kill(pid1, SIGTERM);  // Вежливо просим завершиться
                            waitpid(pid1, NULL, 0);  // Ждем пока завершится
                        }

                        free_words(&words);
                        continue;
                    }
                    else if (pid2 == 0) // успешно - дочерний
                    {
                        close(fd[1]);
                        child_exec(argv2, fd[0], -1, redirect_out, outfile, &words);
                    }

                    close(fd[0]);
                    close(fd[1]);
                    //ждем конца обоих и удаляем их записи из таблицы процессов
                    waitpid(pid1, NULL, 0);
                    waitpid(pid2, NULL, 0); 
                }

                //если не нашли ни одного "|" - то обычная команда
                else
                {
                    execute_command(&words, background, redirect_out, outfile);
                }
                free_words(&words);
            }
        }
        free(input); //очищаем строку
    }
    printf("\nКонец работы.\n");
    return (int)err; //выводит последнюю ошибку (или ERR_OK)
}

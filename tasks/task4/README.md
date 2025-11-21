<h2 align="center">Описание <a href="https://github.com/Alekseeva-Dana/tasks/tree/main/tasks/task4" target="_blank">Task4</h2>
Задание состоит в реализации команд UNIX
<h3 align="center">Реализация <a href="https://github.com/Alekseeva-Dana/tasks/blob/main/tasks/task4/echo.c" tarhet="_blank">echo</h3>
echo печатает свои аргументы через пробелы, также реализованы флаги -n (убрать перенос строки в конце вывода) и -e (поддержка escape-последовательностей)
<pre><code> for (int i = 1; i < argc; i++) {
                if (argv[i][0] == '-') {
                        if (strcmp(argv[i], "-n") == 0) {
                                no_n = 1;
                        }
                        else if (strcmp(argv[i], "-e") == 0) {
                                escapes = 1;
                        }
                }
        }</code></pre>
После определения с флагами считываем каждый следующий элемент и в зависимости от его типа обрабатываем его.
<h3 align="center">Реализация <a href="https://github.com/Alekseeva-Dana/tasks/blob/main/tasks/task4/pwd.c" tarhet="_blank">pwd</h3>
pwd печатает имя текущего каталога.
Основная идея: с помощью команды getcwd получаем полный путь к текущего каталогу, который заносим в динамический буфер
<pre><code> if (getcwd(cwd, max) != NULL) {
                printf("%s\n", cwd);
        }
        else {
                perror("pwd");
                free(cwd);
                return 1;
        }</code></pre>
после вывода освобождаем память
<h3 align="center">Реализация <a href="https://github.com/Alekseeva-Dana/tasks/blob/main/tasks/task4/ls.c" tarhet="_blank">ls</h3>
ls выводит список файлов текущего каталога. Реализованы следующие флаги:
-R – вывести имена всех файлов текущего каталога, а также файлов, содержащихся во
вложенных подкаталогах,
-l – показать атрибуты: тип, права доступа, имя владельца, размер,
-g – показать имя группы владельца.
В реализации используются системные вызовы stat(), lstat(), readdir()
Для работы с большими файлами используем динамическое выделение памяти
Идея: читаем все записи каталогов, пропускаем . и .. на первом уровне, если нужно, увеличиваем массив и копируем имя очередного файла. После чего выводим получившийся список и рекурсивно обходим подкаталоги
<h3 align="center">Реализация <a href="https://github.com/Alekseeva-Dana/tasks/blob/main/tasks/task4/ln.c" tarhet="_blank">ln</h3>
Данная команда создает жесткую или символическую (с флагом -s) ссылку на файл или каталог
Для этого используем команды link и symlink (<unistd.h>).
<pre><code> /* создание ссылки*/
    if (symbolic) {
        /* символическая ссылка */
        if (symlink(original, new_link) == -1) {
            perror("ln");
            return 1;
        }
        printf("Создание символической ссылки: '%s' -> '%s'\n", new_link, original);
    } else {
        /* жесткая ссылка */
        if (link(original, new_link) == -1) {
            perror("ln");
            return 1;
        }
        printf("Создание жесткой ссылки: '%s' -> '%s'\n", new_link, original);
    }
</code></pre>
<h3 align="center">Реализация <a href="https://github.com/Alekseeva-Dana/tasks/blob/main/tasks/task4/grep.c" tarhet="_blank">grep</h3>
Результат: строки файла filename, содержащие substring как подстроку (возможен
флаг - v; в этом случае результат – это строки, которые не содержат substring как
подстроку).
Ищем соответствующие подстроки с помощью функции strstr (<string.h>).
<h3 align="center">Реализация <a href="https://github.com/Alekseeva-Dana/tasks/blob/main/tasks/task4/cat.c" tarhet="_blank">cat</h3>
-просмотр содержимого файлов
Реализована с возможностью использовать флаг -n (сквозная нумерация файлов)
Разбиваем на элементы, заносим в динамический буфер и, если стоит флаг -n, также ведем подсчет слов. Потом выводим их из нашего буфера

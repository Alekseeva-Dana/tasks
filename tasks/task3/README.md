<h2 align="center">Описание <a href="https://github.com/Alekseeva-Dana/tasks/tree/main/tasks/task3" target="_blank">Task3</h2>
Действие программы - разбиваем строки на слова, используя структуру "список слов". Программа выводит в первой строке количество слов, а в каждой следующей - очередное слово (сначала в исходном порядке, затем - в лексикографическом. Обычные символы и спец. символы различаются.
<h3 align="center">Модуль <a href="https://github.com/Alekseeva-Dana/tasks/blob/main/tasks/task3/wordlist.c" tarhet="_blank">wordlist.c</h3>
Модуль wordlist.c содержит функции, которые мы используем в основной программе
<pre><code>WordList* wordlist_create(void)</code></pre>
-создание списка слов
<pre><code>int wordlist_add(WordList *list, const char *word) </code></pre>
-добавление очередного слова в конец списка
<pre><code>size_t wordlist_size(const WordList *list)</code></pre>
-определение размера списка (кол-ва слов в нем)
<pre><code>void wordlist_sort(WordList *list)</code></pre>
-сортировка массива для получения лексикографического порядка (используем пузырьковую сортировку)
<pre><code>void wordlist_print_elements(const WordList *list)</code></pre>
-печать элементов списка(каждый в отдельной строке)
<pre><code>void wordlist_print_with_count(const WordList *list)</code></pre>
-то же самое, что предыдущее, но в первой строке выводится длина списка
<pre><code>void wordlist_destroy(WordList *list)</code></pre>
-высвобождение памяти, занятой списком
<h3 align="center">Модуль <a href="https://github.com/Alekseeva-Dana/tasks/blob/main/tasks/task3/wordlist.h" tarhet="_blank">wordlist.h</h3>
Заголовочный файл, который содержит объявления функций, лежащих в wordlist.c
<h3 align="center">Модуль <a href="https://github.com/Alekseeva-Dana/tasks/blob/main/tasks/task3/main.c" tarhet="_blank">main.c</h3>
<pre><code>int is_word_char(char c)</code></pre>
-определяет, является ли очередной символ обычным
<pre><code>int is_special_char(char c)</code></pre>
-определяет, является ли очередной символ специальным
<pre><code>WordList* parse_line(const char *line) </code></pre>
-разделяет строку на слова по следующему принципу: сначала обрабатываем все специальные слова, состоящие из двух символов, потом - из одного, потом - обычные слова. Добавляем полученное слово в наш список.
<pre><code>char* read_input_line(void)</code></pre>
-чтение входных данных с использованием динамического буфера (когда место заканчивается, переопределяем пространство с помощью realloc)
<b>В основной программе(main):</b>
Читаем очередную строку, разбиваем ее на слова. Сначала выводим список в исходном порядке (в первой строке длина списка), затем - сортируем его и выводим соотвественно в лексикографическом порядке. Очищаем память  завершаем работу, когда наступил "признак конца файла". 

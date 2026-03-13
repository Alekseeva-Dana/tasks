<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
 
    
    
</head>
<body>
    <h1>📊 Graph ADT - библиотека для работы с графами</h1>
    <p>Реализация абстрактного типа данных "Граф" на C++ с базовыми операциями и удобным интерфейсом.</p>
    <h2>🚀 Быстрый старт</h2>
    <h3>Компиляция</h3>
    <pre><code>g++ -o graph main.cpp</code></pre>
    <h3>Запуск</h3>
    <pre><code>./graph        # интерактивный режим
./graph test   # режим тестирования</code></pre>
    <h2>📋 Возможности</h2>
    <ul>
        <li>✅ Создание ориентированных графов</li>
        <li>✅ Добавление и удаление вершин</li>
        <li>✅ Добавление и удаление ребер с метками</li>
        <li>✅ Объединение графов</li>
        <li>✅ Проверка достижимости вершин (DFS)</li>
        <li>✅ Поиск сильно связных компонент</li>
        <li>✅ Копирование и присваивание графов</li>
        <li>✅ Удобный операторный интерфейс</li>
    </ul>
    <h2>📚 Интерактивные команды</h2>
    <table>
        <tr>
            <th>Команда</th>
            <th>Описание</th>
            <th>Пример</th>
        </tr>
        <tr>
            <td><code>addV &lt;имя&gt;</code></td>
            <td>Добавить вершину</td>
            <td><code>addV Moscow</code></td>
        </tr>
        <tr>
            <td><code>addE &lt;откуда&gt; &lt;куда&gt; [метка]</code></td>
            <td>Добавить ребро</td>
            <td><code>addE Moscow SPb train</code></td>
        </tr>
        <tr>
            <td><code>removeV &lt;имя&gt;</code></td>
            <td>Удалить вершину</td>
            <td><code>removeV Moscow</code></td>
        </tr>
        <tr>
            <td><code>removeE &lt;откуда&gt; &lt;куда&gt; [метка]</code></td>
            <td>Удалить ребро</td>
            <td><code>removeE Moscow SPb</code></td>
        </tr>
        <tr>
            <td><code>print</code></td>
            <td>Вывести граф</td>
            <td><code>print</code></td>
        </tr>
        <tr>
            <td><code>reach &lt;откуда&gt; &lt;куда&gt;</code></td>
            <td>Проверить достижимость</td>
            <td><code>reach Moscow SPb</code></td>
        </tr>
        <tr>
            <td><code>outgoing &lt;вершина&gt;</code></td>
            <td>Вывести исходящие ребра</td>
            <td><code>outgoing Moscow</code></td>
        </tr>
        <tr>
            <td><code>components</code></td>
            <td>Найти сильно связные компоненты</td>
            <td><code>components</code></td>
        </tr>
        <tr>
            <td><code>quit</code></td>
            <td>Выход</td>
            <td><code>quit</code></td>
        </tr>
    </table>
    <h2>💻 Пример использования в коде</h2>
    <pre><code>#include "Graph.h"

int main() {
    Graph g;
    // Добавление вершин (разные способы)
    g.addVertex("A");
    g + "B" + "C";  // через оператор
    // Добавление ребер
    g.addEdge("A", "B", "friend");
    g.addEdge("B", "C", "knows");
    g.addEdge("C", "A", "colleague");
    // Проверка достижимости
    if (g.isReachable("A", "C")) {
        std::cout << "A может достичь C" << std::endl;
    }
    // Вывод графа
    std::cout << g << std::endl;
    // Поиск сильно связных компонент
    g.findStronglyConnectedComponents();
    // Удаление вершины
    g - "B";
    // Объединение графов
    Graph g2;
    g2 + "D" + "E";
    g + g2;  // теперь g содержит A, C, D, E
    return 0;
}</code></pre>
    <h2>🔧 Основные методы</h2>
    <table>
        <tr>
            <th>Метод</th>
            <th>Описание</th>
        </tr>
        <tr>
            <td><code>addVertex(const char* name)</code></td>
            <td>Добавляет вершину</td>
        </tr>
        <tr>
            <td><code>addEdge(const char* from, const char* to, const char* label = "")</code></td>
            <td>Добавляет ребро</td>
        </tr>
        <tr>
            <td><code>removeVertex(const char* name)</code></td>
            <td>Удаляет вершину и все связанные ребра</td>
        </tr>
        <tr>
            <td><code>removeEdge(const char* from, const char* to, const char* label = "")</code></td>
            <td>Удаляет ребро</td>
        </tr>
        <tr>
            <td><code>isReachable(const char* from, const char* to)</code></td>
            <td>Проверяет достижимость</td>
        </tr>
        <tr>
            <td><code>findStronglyConnectedComponents()</code></td>
            <td>Находит сильно связные компоненты</td>
        </tr>
        <tr>
            <td><code>contains(const char* name)</code></td>
            <td>Проверяет наличие вершины</td>
        </tr>
    </table>
    <h2>🎯 Операторы</h2>
    <table>
        <tr>
            <th>Оператор</th>
            <th>Описание</th>
            <th>Пример</th>
        </tr>
        <tr>
            <td><code>+</code></td>
            <td>Добавление вершины или объединение графов</td>
            <td><code>g + "A" + "B"; g1 + g2;</code></td>
        </tr>
        <tr>
            <td><code>-</code></td>
            <td>Удаление вершины</td>
            <td><code>g - "A";</code></td>
        </tr>
        <tr>
            <td><code>&lt;&lt;</code></td>
            <td>Вывод графа в поток</td>
            <td><code>std::cout &lt;&lt; g;</code></td>
        </tr>
        <tr>
            <td><code>=</code></td>
            <td>Присваивание графов</td>
            <td><code>g1 = g2;</code></td>
        </tr>
    </table>
    <div class="note">
        <strong>📌 Особенности:</strong>
        <ul>
            <li>Граф ориентированный</li>
            <li>Ребра могут иметь метки</li>
            <li>Вершины идентифицируются по именам (строки)</li>
            <li>При добавлении ребра вершины создаются автоматически</li>
            <li>Дубликаты вершин и ребер не создаются</li>
        </ul>
    </div>
    <div class="warning">
        <strong>⚠️ Ограничения реализации:</strong>
        <ul>
            <li>Максимум 100 вершин (ограничение в алгоритмах)</li>
            <li>Конструктор от строки работает только с числами</li>
            <li>Нет проверки на циклические зависимости при удалении</li>
            <li>Оператор delete[] реализован нестандартно</li>
        </ul>
    </div>
    <h2>📁 Структура проекта</h2>
    <pre><code>.
├── Graph.h    # Основной заголовочный файл с реализацией графа
└── main.cpp   # Тесты и интерактивный интерфейс</code></pre>
    <h2>🧪 Запуск тестов</h2>
    <pre><code>./graph test</code></pre>
    <p>Тесты проверяют все основные возможности:</p>
    <ul>
        <li>Создание графов (пустой, из строки, копированием)</li>
        <li>Добавление и удаление вершин и ребер</li>
        <li>Операторы + и -</li>
        <li>Проверку достижимости</li>
        <li>Поиск сильно связных компонент</li>
    </ul>
    <hr>
    <p style="text-align: center; color: #6c757d;">Graph ADT - простая и удобная библиотека для работы с графами на C++</p>
</body>
</html>

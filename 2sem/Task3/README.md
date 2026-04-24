<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
   
</head>
<body>
    <h1>🚗 Task 3: Иерархия классов (Транспортные средства)</h1>

    
Task3/
├── include/
│   ├── Transport.h          # Абстрактный базовый класс
│   ├── Car.h                # Легковой автомобиль
│   ├── Truck.h              # Грузовик
│   ├── ElectricCar.h        # Электромобиль
│   └── HybridCar.h          # Гибрид (множественное наследование)
├── src/
│   ├── Transport.cpp
│   ├── Car.cpp
│   ├── Truck.cpp
│   ├── ElectricCar.cpp
│   └── HybridCar.cpp
└── main.cpp                  # Пользовательский интерфейс + тест
    <h2>🎯 Описание</h2>
    <p>Программа демонстрирует иерархию классов транспортных средств с использованием абстрактного класса и множественного наследования.</p>
    <h3>Базовый абстрактный класс <code>Transport</code></h3>
    <ul>
        <li>Чистая виртуальная функция: <code>calculateMaxCargoWeight()</code></li>
        <li>Виртуальная функция: <code>displayInfo()</code></li>
    </ul>
    <h3>Классы-наследники</h3>
    <ul>
        <li><code>Car</code> - легковой автомобиль</li>
        <li><code>Truck</code> - грузовик</li>
        <li><code>ElectricCar</code> - электромобиль</li>
        <li><code>HybridCar</code> - гибрид (наследуется от Car и ElectricCar) ✅ множественное наследование</li>
    </ul>
    <h2>✅ Выполненные требования</h2>
    <ul>
        <li>✅ 8+ файлов (фактически 10)</li>
        <li>✅ Абстрактный класс с чистой виртуальной функцией</li>
        <li>✅ Множественное наследование (HybridCar)</li>
        <li>✅ Обоснованное использование protected</li>
        <li>✅ Пользовательский интерфейс (меню)</li>
        <li>✅ Полиморфный тест через указатель на базовый класс</li>
    </ul>
    <h2>🔧 Компиляция и запуск</h2>
    <pre>
g++ -std=c++11 main.cpp src/*.cpp -o task3
./task3
    </pre>
    <h2>🎮 Управление</h2>
    <pre>
1 - Добавить Car
2 - Добавить Truck  
3 - Добавить ElectricCar
4 - Добавить HybridCar
5 - Показать все ТС
6 - Полиморфный тест (суммарная грузоподъемность)
7 - Удалить ТС
8 - Очистить список
0 - Выход
    </pre>
    <h2>📝 Пример работы</h2>
    <pre>
--- All Vehicles ---
1. Model: Tesla Model 3, Year: 2023, Type: Electric Car, Battery: 75kWh, Battery weight: 480kg | Cargo capacity: 0.116t
2. Model: Toyota Camry, Year: 2022, Type: Car, Passengers: 5, Trunk volume: 500L | Cargo capacity: 0.375t
========== POLYMORPHIC TEST ==========
Total cargo capacity of all vehicles: 0.491 tons
    </pre>
</body>
</html>

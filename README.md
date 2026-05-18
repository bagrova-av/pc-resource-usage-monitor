# PC Resource Monitor — Real-time System Metrics Dashboard for Linux

Приложение представляет собой монитор ресурсов системы для Linux (аналог `top`/`htop`), отображающий загрузку процессора (CPU) и оперативной памяти (RAM) в режиме реального времени через веб-интерфейс.

## Архитектура проекта
```text
.
├── app
│   ├── Application
│   │   ├── Application.cpp
│   │   └── Application.h
│   ├── main.cpp
│   ├── SystemMonitor
│   │   ├── SystemMonitor.cpp
│   │   └── SystemMonitor.h
│   └── WebServer
│       ├── WebServer.cpp
│       └── WebServer.h
├── CMakeLists.txt
├── README.md
└── webUI
    ├── index.html
    ├── script.js
    ├── script.ts
    └── style.css
```

### Основные компоненты:

- **Application Layer**: класс `Application` управляет жизненным циклом программы и координирует работу компонентов;
- **System Domain**: класс `SystemMonitor` отвечает за парсинг виртуальной файловой системы `/proc` в Linux;
- **Web Domain**: класс `WebServer` реализует REST API для передачи данных в формате JSON;
- **Frontend**: одностраничное приложение (SPA) на TypeScript, взаимодействующее с Backend через асинхронные HTTP-запросы.

#### Особенности:
- **CPU Load**: Рассчитывается как разница (delta) между двумя снимками состояния `/proc/stat`. Это позволяет отображать мгновенную нагрузку, а не среднюю за всё время работы ОС.
- **RAM Info**: Для точности используется значение `MemAvailable` из `/proc/meminfo`, так как оно лучше отражает реальное количество свободной памяти в Linux, чем `MemFree` (учитывая кэш и буферы).
- **SVG Gauges**: Визуализация выполнена на SVG-дугах. Длина полоски управляется через CSS свойство `stroke-dashoffset`.

### Завершение приложения
Приложение завершает работу по сигналу `SIGINT` (**Ctrl+C**). При получении сигнала приложение:
- Останавливает сетевой сервер и освобождает порт 8080.
- Корректно завершает работу всех потоков.
- Выводит прощальное сообщение в терминал.ы


### Стек технологий и зависимости
- **Backend**: C++17
- **Система сборки**: CMake ≥ 3.14
- **Компилятор**: g++ ≥ 9
- **ОС**: Linux (используются системные вызовы и сигналы Linux)
- **Frontend**: TypeScript, HTML5 (SVG), CSS3

### Зависимости
| Библиотека | Версия | Тип подключения |
|------------|--------|-----------------|
| `nlohmann/json` | 3.12.0 | Автоматическое через FetchContent при запуске `cmake ..` |
| `cpp-httplib` | 0.14.3 | Автоматическое через FetchContent при запуске `cmake ..` |

- [yhirose/cpp-httplib](https://github.com/yhirose/cpp-httplib) — многопоточный HTTP-сервер.
- [nlohmann/json](https://github.com/nlohmann/json) — работа с JSON в стиле C++.

## Инструкция по сборке и запуску

### 1. Сборка Frontend
Перейдите в папку с webUI и скомпилируйте TypeScript:
```bash
cd webUI
tsc script.ts --target es6
```

### 2. Сборка Backend
Вернитесь в корень проекта и выполните:
```bash
# Создание директории сборки
mkdir build
cd build

# Конфигурация и компиляция
cmake ..
# Для сборки в однопоточном режиме:
cmake --build .
# Для сборки в многопоточном режиме:
# N - целое число потоков, используемых при сборке
cmake --build . -jN
```

Папка `webUI` должна находиться в той же директории, откуда запускается исполняемый файл, поэтому выполните:
```bash
cp -r ../webUI .
```

### Запуск
```bash
./PcResourceMonitor    
```

После запуска откройте в браузере: `http://localhost:8080`
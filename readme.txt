# Предварительные требования

# Windows 10/11 x64

# Visual Studio 2022 с компонентами: Desktop development with C++ (MSVC v143, C++ CMake tools)

# Git, CMake 3.23+ (идёт с VS), PowerShell

# СIMS клиент-серверное C++-приложение с GUI на Qt и серверной частью на Boost.Asio. 

# Проект собирается через CMakePresets и использует vcpkg (как сабмодуль в external/vcpkg) для управления зависимостями. 

# В репозитории есть манифест vcpkg.json, а также преднастроенные пресеты сборки для клиента и сервера. 

# Qt-клиент: графический интерфейс для работы с данными и взаимодействия с сервером.

# Сервер: многопоточный TCP-сервер на Boost.Asio для приёма/отправки данных.

# Логирование через glog.

# Повторяемая сборка на Windows 10/11 + VS2022 с использованием локального vcpkg-сабмодуля.

# Архитектура

CIMS/
├─ external/
│  └─ vcpkg/                 ← сабмодуль vcpkg (локальный тулчейн + зависимости)
│
├─ client_qt/                ← Qt-клиент (GUI)
│  ├─ include/               ← заголовки клиента
│  ├─ src/                   ← исходники клиента (окна/виджеты, логика, сетевой клиент)
│  └─ CMakeLists.txt
│
├─ include/                  ← заголовки сервера
│  ├─ handler.hpp
│  ├─ heapSort.hpp
│  ├─ server.hpp
|  ├─ session.hpp
│  ├─ heapSort.hpp			← алгоритм сортировки
|  └─ queue.hpp				← шаблонный класс очереди
│
├─ src/                      ← исходники сервера 
│  ├─ main.cpp               ← вход сервера
│  ├─ server.cpp             ← accept/listen
│  ├─ session.cpp            ← обработка подключения (TCP)
│  └─ handler.cpp            ← роутинг/обработка сообщений               
│
├─ build/                    ← артефакты CMake/VS (генерируется)
│
├─ CMakeLists.txt            ← корневой CMake
├─ CMakePresets.json         ← пресеты сборки (qt-client-vs2022-debug, server-vs2022-debug)
├─ vcpkg.json                ← манифест зависимостей (glog, boost-*)
├─ ext_install.bat           ← автоматизация: init vcpkg + сборка в нужном порядке
└─ readme.txt / README.md    ← описание, инструкции


# Технологии

# C++20, Qt, Boost (asio, json и др.), glog, CMake, vcpkg (manifest). 
# vcpkg.json обеспечивает автоматическую установку зависимостей при конфигурации через CMake. 

# Сборка
# Запусти ext_install.bat
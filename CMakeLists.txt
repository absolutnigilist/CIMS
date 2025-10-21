cmake_minimum_required(VERSION 3.20)
project(cims LANGUAGES CXX)

# Общие настройки
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Что собирать
option(BUILD_SERVER "Build TCP server" ON)
option(BUILD_QT_CLIENT "Build Qt GUI client" ON)

# --------- Сервер ---------
if (BUILD_SERVER)
  # Заголовки проекта
  include_directories(${CMAKE_SOURCE_DIR}/include)

  # Зависимости через vcpkg (CONFIG-режим)
  find_package(glog CONFIG REQUIRED)
  find_package(Boost CONFIG REQUIRED COMPONENTS json asio)

  # Источники сервера
  add_executable(boost_tcp_server
    src/main.cpp
    src/server.cpp
    src/session.cpp
    src/handler.cpp
  )

  # Директория логов рядом с бинарником
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)
  target_compile_definitions(boost_tcp_server PRIVATE LOG_DIR="${CMAKE_BINARY_DIR}/logs")

  # Windows min target
  if (WIN32)
    target_compile_definitions(boost_tcp_server PRIVATE _WIN32_WINNT=0x0A00)
  endif()

  # Линки
  target_link_libraries(boost_tcp_server PRIVATE
    glog::glog
    Boost::json
    Boost::asio
  )

  # pthread для POSIX
  if (NOT WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(boost_tcp_server PRIVATE Threads::Threads)
  endif()
endif()

# --------- Qt-клиент ---------
if (BUILD_QT_CLIENT)
  add_subdirectory(client_qt)
endif()

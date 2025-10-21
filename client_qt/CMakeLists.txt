cmake_minimum_required(VERSION 3.20)
project(queue_client LANGUAGES CXX)

# ----------------------------------------
# Настройка компиляции
# ----------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Для MSVC: исходники в UTF-8
if (MSVC)
  add_compile_options(/utf-8)
endif()

# Qt будет найден через vcpkg toolchain (см. CMakePresets.json)
find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets Network)

# ----------------------------------------
# Файлы
# ----------------------------------------
set(QUEUE_CLIENT_SOURCES
    src/main.cpp
    src/MainWindow.cpp  
    src/TcpClient.cpp
)

set(QUEUE_CLIENT_HEADERS
    include/MainWindow.hpp
    include/TcpClient.hpp
    include/Json.hpp
)

# ----------------------------------------
# Таргет (GUI на Windows через WIN32)
# ----------------------------------------
add_executable(queue_client WIN32
    ${QUEUE_CLIENT_SOURCES}
    ${QUEUE_CLIENT_HEADERS}
)

# Собственные include
target_include_directories(queue_client PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(queue_client PRIVATE 
    Qt6::Widgets 
    Qt6::Network
)
# --- Копируем Qt platform plugin "windows" переносимым способом ---
# Работает и с vcpkg manifest: путь берётся из таргета Qt6::QWindowsIntegrationPlugin
add_custom_command(TARGET queue_client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
          "$<TARGET_FILE_DIR:queue_client>/platforms"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "$<TARGET_FILE:Qt6::QWindowsIntegrationPlugin>"
          "$<TARGET_FILE_DIR:queue_client>/platforms/"
)

# Windows: только GUI-подсистема (без консольного окна)
if (MINGW)
  target_link_options(queue_client PRIVATE ws2_32)
endif()
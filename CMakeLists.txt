cmake_minimum_required (VERSION 3.8)
project(boost_tcp_server LANGUAGES CXX)

# Настройка компиляции
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE Debug)

# Добавляем путь к локальном Boost
set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/external/boost_1_86_0")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")

# Подключаем заголовочные файлы 
include_directories(${BOOST_INCLUDEDIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Спиок исходных файлов
set(SOURCES
	src/main.cpp
	src/server.cpp 
	src/session.cpp 
)
# Создание исполняемого файла
add_executable(boost_tcp_server ${SOURCES})

# Линковка с Boost
target_link_libraries(boost_tcp_server PRIVATE pthread)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
  "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,
  $<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,
  $<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

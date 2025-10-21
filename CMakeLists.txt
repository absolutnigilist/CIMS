cmake_minimum_required(VERSION 3.15)
project(boost_tcp_server LANGUAGES CXX)

# ----------------------------------------
# Настройка компиляции
# ----------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE Debug)

# ----------------------------------------
# Пути к внешним библиотекам
# ----------------------------------------
# Boost (header-only)
set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/external/boost_1_89_0")
include_directories(${BOOST_ROOT})
link_directories(${BOOST_ROOT}/stage/lib)

# Glog (DLL)
set(GLOG_INSTALL_DIR "${CMAKE_SOURCE_DIR}/external/glog")
set(glog_DIR "${GLOG_INSTALL_DIR}/lib/cmake/glog")
set(CMAKE_PREFIX_PATH "${GLOG_INSTALL_DIR};${CMAKE_PREFIX_PATH}")
find_package(glog REQUIRED)

# Собственные include
include_directories(${CMAKE_SOURCE_DIR}/include)

# ----------------------------------------
# Список исходников
# ----------------------------------------
set(SOURCES
    src/main.cpp
    src/server.cpp
    src/session.cpp
    src/handler.cpp
)

# ----------------------------------------
# Создание исполняемого файла
# ----------------------------------------
add_executable(boost_tcp_server ${SOURCES})

# Линковка с Glog
target_link_libraries(boost_tcp_server PRIVATE glog::glog libboost_json)

# Windows 10
add_definitions(-D_WIN32_WINNT=0x0A00)

# Линковка с pthread для Linux/Unix
if(NOT WIN32)
    target_link_libraries(boost_tcp_server PRIVATE pthread)
endif()

# ----------------------------------------
# Включение горячей перезагрузки для MSVC
# ----------------------------------------
if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
        "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,"
        "$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,"
        "$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# ----------------------------------------
# Копирование DLL glog рядом с exe
# ----------------------------------------
add_custom_command(TARGET boost_tcp_server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GLOG_INSTALL_DIR}/bin/glog.dll"
        $<TARGET_FILE_DIR:boost_tcp_server>
)

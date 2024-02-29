cmake_minimum_required(VERSION 3.10)

project(YourProjectName)

# Укажите исходные файлы вашего проекта
file(GLOB SOURCES "src/*.cpp")

# Установка переменной CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH "/usr/local;/usr;/usr/x86_64-linux-gnu;/usr/lib/x86_64-linux-gnu")

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# Укажите, в какие исполняемые файлы должны быть скомпилированы ваши исходные файлы
add_executable(control src/control.cpp)
add_executable(calc src/calc.cpp)

# Линковка библиотек к исполняемым файлам
target_link_libraries(control PRIVATE ${ZMQ_LIBRARIES})
target_link_libraries(calc PRIVATE ${ZMQ_LIBRARIES})
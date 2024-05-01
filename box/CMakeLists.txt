cmake_minimum_required(VERSION 3.10)

set(project_name client.out)
project(${project_name} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_compile_options("-Wall" "-Wextra" "-I /usr/include/c++13/")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(QT_VERSION_MAJOR 6)
find_package(QT NAMES Qt6 COMPONENTS Core Gui Widgets Qml Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick REQUIRED)

add_executable(${project_name}
  main.cpp
  box.cpp
  Taskpool.cpp
  loger.cpp
  client.cpp
)
target_link_libraries(${project_name} Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${project_name} Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(${project_name} Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${project_name} Qt${QT_VERSION_MAJOR}::Quick)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/)
cmake_minimum_required(VERSION 3.10)

set(project_name event.out)
project(${project_name} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_compile_options("-Wall" "-Wextra" "-lpthread")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(QT_VERSION_MAJOR 5)
find_package(QT NAMES Qt5 COMPONENTS Core Gui Widgets Qml Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(${project_name}
  main.cpp
)
target_link_libraries(${project_name} Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${project_name} Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(${project_name} Qt${QT_VERSION_MAJOR}::Widgets)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/)
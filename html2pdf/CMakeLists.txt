cmake_minimum_required(VERSION 3.10)

set(project_name html2pdf)
project(${project_name} LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
add_compile_options("-Wall" "-Wextra" "-lpthread")

find_package(Qt6 COMPONENTS Core Gui WebEngineWidgets REQUIRED)
add_executable(${project_name} main.cpp)
target_link_libraries(${project_name} Qt6::Core Qt6::Gui Qt6::WebEngineWidgets)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/)
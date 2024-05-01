## A very confusing bug
This is the CMakeLists before modify
```
...
set(project_dir "${CMAKE_SOURCE_DIR}/code")
include_directories("${project_dir}/include")
file(GLOB SOURCES "${project_dir}/*.cpp")
add_executable(${project_name} ${SOURCES})
...
```
after modify
```
set(project_dir "${CMAKE_SOURCE_DIR}/")
# include_directories("${project_dir}/include")
file(GLOB SOURCES "${project_dir}/*.cpp")
add_executable(${project_name} ${SOURCES})
```
then it works with no error named `undefined reference to xxx::staticmetaobject`
the cmake cmd `include_directories` seems to make something broken
# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (SimpleMon)
#set (CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-std=c++20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTATIC_CONCPP")
# Create a sources variable with a link to all cpp files to compile
set(SERVER_SOURCES
    src/server/server.cpp
    src/server/sql_handler.cpp
    src/config.cpp
)
set(CLIENT_SOURCES
    src/client/client.cpp
    src/config.cpp
)

# Add an executable with the above sources
add_executable(server ${SERVER_SOURCES})
add_executable(client ${CLIENT_SOURCES})


execute_process (
    COMMAND bash -c "mysql_config --libs"
    OUTPUT_VARIABLE mysqlLibs
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(server 
    pthread
    mysqlcppconn
    mysqlcppconn-static
)

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(server
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/server
)
target_include_directories(client
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/client
)

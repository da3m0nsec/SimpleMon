# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (SimpleMon)
include(FindBotan.cmake)

# glib (notify)
FIND_PACKAGE(PkgConfig REQUIRED)

PKG_CHECK_MODULES(GLIB REQUIRED glib-2.0)
PKG_CHECK_MODULES(LIBNOTIFY QUIET libnotify)
#####

add_compile_options(-std=c++17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTATIC_CONCPP -lssl -lcrypto")

# Create a sources variable with a link to all cpp files to compile
set(SERVER_SOURCES
    src/server/server.cpp
    src/server/sql_handler.cpp
    src/server/socket_server.cpp
    src/server/ssl_server.cpp
    src/server/alerts.cpp
    src/config.cpp
)
set(CLIENT_SOURCES
    src/client/client.cpp
    src/client/socket_client.cpp
    src/client/ssl_client.cpp
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
#set(OPENSSL_USE_STATIC_LIBS TRUE)
#find_package(OpenSSL REQUIRED)

#find_package(OpenSSL REQUIRED)

target_link_libraries(server 
    Threads::Threads
    mysqlcppconn
    mysqlcppconn-static
    ${BOTAN2_LIBRARIES}
    ${GLIB_LIBRARY_DIRS}
    ${NOTIFY_LIBRARIES}
)

target_link_libraries(client 
    ${BOTAN2_LIBRARIES}
)


# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(server
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/server
        ${BOTAN2_INCLUDE_DIRS}
        ${GLIB_INCLUDE_DIRS}
        #glib (notify)
        /usr/include/gdk-pixbuf-2.0 
        ${NOTIFY_INCLUDE_DIRS}
)
target_include_directories(client
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/client
        ${BOTAN2_INCLUDE_DIRS}

)

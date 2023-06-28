cmake_minimum_required(VERSION 3.10.2)
project(bibtgbot)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)
include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

#add_library(doctest INTERFACE)
#target_include_directories(doctest INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(doctest.h)
#find_package(doctest REQUIRED)

add_executable(bibtgbot bibtgbot.cpp)
add_executable(test2 test.cpp)

#set_target_properties(bibtgbot PROPERTIES LINK_FLAGS "-e mains")

target_link_libraries(bibtgbot /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})
target_link_libraries(test2 ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})

#target_compile_features(test2 PRIVATE cxx_std_14)

enable_testing()
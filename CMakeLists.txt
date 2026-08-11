cmake_minimum_required(VERSION 3.20)

project(SHORT_URL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()

add_executable(SHORT_URL
    main.cpp
    generator.h
    id_storage.h
    db_storage.h)

add_subdirectory(
    ${CMAKE_SOURCE_DIR}/libs/SQLiteCpp
    ${CMAKE_BINARY_DIR}/SQLiteCpp
)

target_include_directories(SHORT_URL PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/cpp-httplib
    ${CMAKE_SOURCE_DIR}/libs/SQLiteCpp/include
)

target_link_libraries(SHORT_URL PRIVATE
    SQLiteCpp
)

if(WIN32)
    target_link_libraries(SHORT_URL PRIVATE
        ws2_32
    )
endif()
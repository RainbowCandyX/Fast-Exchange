cmake_minimum_required(VERSION 3.16)
project(FastExchange VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

add_executable(FastExchange WIN32 MACOSX_BUNDLE
    main.cpp
    exchangewidget.h
    exchangewidget.cpp
)

target_link_libraries(FastExchange PRIVATE Qt6::Widgets Qt6::Network)

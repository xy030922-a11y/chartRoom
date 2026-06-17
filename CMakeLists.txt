cmake_minimum_required(VERSION 3.16)

project(QtChatClient VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

add_executable(QtChatClient
    main.cpp
    ui/MainWindow.h
    ui/MainWindow.cpp
    server/ClientSocket.h
    server/ClientSocket.cpp
    server/MessageHandler.h
    server/MessageHandler.cpp
)

target_include_directories(QtChatClient PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/server
)

target_link_libraries(QtChatClient PRIVATE
    Qt6::Widgets
    Qt6::Network
)


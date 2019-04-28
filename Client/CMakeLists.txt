cmake_minimum_required(VERSION 3.14)
project(bmstu_hw_5_client)

set(CMAKE_CXX_STANDARD 14)

find_package(
        Qt5
        REQUIRED COMPONENTS
        Core
        Widgets
        Network
        WebSockets
)

add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOUIC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTORCC TRUE)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Widgets Qt5::Network Qt5::WebSockets)
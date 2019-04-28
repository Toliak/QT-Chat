project(bmstu_hw_5_server)

set(CMAKE_CXX_STANDARD 14)

find_package(
        Qt5
		# HINTS "$ENV{QTDIR}"
        REQUIRED COMPONENTS
        Core
        Network
        WebSockets
)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        Config.h
        Config.cpp
        Listener.cpp
        Listener.h
		MessageHandler.cpp
        MessageHandler.h
        Client.cpp
        Client.h
        ChatListener.cpp
        ChatListener.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
# set_target_properties(${PROJECT_NAME} PROPERTIES AUTOUIC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTORCC TRUE)

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        Qt5::Core
        Qt5::Network
        Qt5::WebSockets
)


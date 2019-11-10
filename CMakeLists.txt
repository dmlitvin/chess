cmake_minimum_required(VERSION 3.15)

project(chess)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-Wall -Wextra -Werror)

add_executable(server_chess server.cpp Transporter.cpp ChessServer.cpp ChessMessage.cpp)
add_executable(client_chess client.cpp Transporter.cpp ChessMessage.cpp )

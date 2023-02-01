cmake_minimum_required(VERSION 3.0.0)

add_executable(server server.cpp)

target_link_libraries(server PRIVATE librt.so)
set( CMAKE_CXX_FLAGS " -pthread -lrt -lpthread -lboost_thread " )

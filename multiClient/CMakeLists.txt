cmake_minimum_required(VERSION 3.0.0)

add_executable(client client.cpp)

target_link_libraries(client PRIVATE librt.so)
set( CMAKE_CXX_FLAGS " -pthread -lrt -lpthread -lboost_thread " )

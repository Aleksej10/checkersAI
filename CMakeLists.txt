cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(checkers)
list(APPEND CMAKE_PREFIX_PATH "$HOME/tmp/libtorch/")

find_package(Torch REQUIRED)
find_package(Threads REQUIRED)
set(CMAKE_CXX_FLAGS "-g -std=c++17 ${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_library(pos STATIC pos.cpp)
add_library(model STATIC model.cpp)
add_library(node STATIC node.cpp)

target_link_libraries(pos "${TORCH_LIBRARIES}")
target_link_libraries(node "${TORCH_LIBRARIES}")
target_link_libraries(model "${TORCH_LIBRARIES}")
target_link_libraries(node pos model)

add_executable(self_train train.cpp pos.cpp node.cpp model.cpp)
add_executable(random_train train_vs_random.cpp pos.cpp node.cpp model.cpp)
add_executable(test_clean test_clean.cpp model.cpp)
add_executable(server server.cpp model.cpp node.cpp pos.cpp)

target_link_libraries(self_train "${TORCH_LIBRARIES}")
target_link_libraries(random_train "${TORCH_LIBRARIES}")
target_link_libraries(test_clean "${TORCH_LIBRARIES}")
target_link_libraries(server "${TORCH_LIBRARIES}" Threads::Threads)

set_property(TARGET self_train PROPERTY CXX_STANDARD 17)
set_property(TARGET random_train PROPERTY CXX_STANDARD 17)
set_property(TARGET test_clean PROPERTY CXX_STANDARD 17)
set_property(TARGET server PROPERTY CXX_STANDARD 17)

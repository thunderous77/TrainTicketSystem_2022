cmake_minimum_required (VERSION 2.8)
project(TrainticketSystem)
set(CMAKE_CXX_FLAGS "-ftrapv -fsanitize=address -fsanitize=undefined")
add_executable(code main.cpp Order.cpp User.cpp Trainticket.cpp)
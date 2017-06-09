project (haffman)

add_library(haffman_small_data STATIC lib.cpp)

add_executable(haffman haffman.cpp)
#add_executable(decode decode.cpp)


target_link_libraries(haffman haffman_small_data)	
#target_link_libraries(decode haffman_small_data)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

#cmake . -DCMAKE_BUILD_TYPE=Debug

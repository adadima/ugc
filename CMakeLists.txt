cmake_minimum_required(VERSION 3.19)

project(ugc C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ugc SHARED src/ugc.c)
target_include_directories(ugc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
install(DIRECTORY include DESTINATION .)
install(TARGETS ugc DESTINATION lib)

add_executable(ugc_example EXCLUDE_FROM_ALL example.c)
target_link_libraries(ugc_example ugc c)


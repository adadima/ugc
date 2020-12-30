cmake_minimum_required(VERSION 3.19)

project(ugc C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ugc_lib_sources
        src/ugc.c)

if (APPLE)
    list(APPEND ugc_lib_sources
            src/growheap_darwin.c)
elseif (LINUX)
    list(APPEND ugc_lib_sources
            src/growheap_linux.c)
else()
    message(FATAL "unsupported platform: Please use Linux or Darwin")
endif ()

add_library(ugc SHARED ${ugc_lib_sources})
target_include_directories(ugc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(DIRECTORY include DESTINATION .)
install(TARGETS ugc DESTINATION lib)

add_executable(ugc_example EXCLUDE_FROM_ALL example.c)
target_link_libraries(ugc_example ugc c)


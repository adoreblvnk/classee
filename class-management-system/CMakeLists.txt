# min cmake version needed
cmake_minimum_required(VERSION 3.10)

# proj name & lang
project(ClassManagementSystem C)

# use c99 standard & enforce it
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# find headers in /include dir
include_directories(include)

# get all .c files from /src dir
file(GLOB SRC_FILES "src/*.c")

# create 'cms' executable from src files
add_executable(cms ${SRC_FILES})

# clean dir best practices

# put final executable in /bin dir (eg bin/cms)
set_target_properties(cms PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# put object files & other build stuff in /build
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

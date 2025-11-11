cmake_minimum_required(VERSION 3.13)
project(ClassManagementSystem C)

# use C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
# output dir is `build/bin`
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set all source files (files without a main func)
set(CMS_SOURCES
  src/command_parser.c
  src/database.c
  src/summary.c
  src/services/log.c
  src/services/journal.c
  src/utils/file_utils.c
  src/utils/str_utils.c
  src/utils/print_util.c
)

# build `cms` executable
add_executable(cms src/main.c ${CMS_SOURCES})
target_include_directories(cms PUBLIC include)
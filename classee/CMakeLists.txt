cmake_minimum_required(VERSION 3.13)
project(classee C)

# use C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
# output dir is `build/bin`
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set all source files (files without a main func)
set(CLASSEE_SOURCES
  src/command_parser.c
  src/database.c
  src/summary.c
  src/services/log.c
  src/services/transaction_log.c
  src/utils/file_utils.c
  src/utils/general_utils.c
  src/utils/print_util.c
  src/utils/str_utils.c
)

# build `cms` executable
add_executable(classee src/main.c ${CLASSEE_SOURCES})
target_include_directories(classee PUBLIC include)

# build unit tests
# option(BUILD_TESTING "Build unit tests" ON) # off for windows
if(BUILD_TESTING)
  # enable `ctest` for unit testing
  enable_testing()
  include(FetchContent)

  # declare & download Unity test framework
  FetchContent_Declare(
    Unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.5.2
    SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/tests/unity
  )
  FetchContent_MakeAvailable(Unity)

  # build `run_tests` executable
  add_executable(run_tests
    tests/test_runner.c # test runner w main func
    tests/test_file_utils.c
    tests/test_str_utils.c
    ${CLASSEE_SOURCES}
  )

  # Define the TESTING macro only for the run_tests target
  target_compile_definitions(run_tests PRIVATE TESTING)

  # look for headers inside `include` dir & `tests` dir (for Unity)
  target_include_directories(run_tests PUBLIC include PRIVATE tests)
  # use `unity` lib when linking
  target_link_libraries(run_tests PRIVATE unity)

  # register run_tests with CTest
  add_test(
    NAME UnitTests 
    COMMAND $<TARGET_FILE:run_tests>
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endif()

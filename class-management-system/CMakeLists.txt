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
  src/cms.c
  src/log.c
  src/journal.c
  src/summary.c
  src/utils.c
)

# build `cms` executable
add_executable(cms src/main.c ${CMS_SOURCES})
target_include_directories(cms PUBLIC include)

# build unit tests
option(BUILD_TESTING "Build unit tests" ON)
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
    tests/test_utils.c
    ${CMS_SOURCES}
  )

  # look for headers inside `include` dir & `tests` dir (for Unity)
  target_include_directories(run_tests PUBLIC include PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
  # use `unity` lib when linking
  target_link_libraries(run_tests PRIVATE unity)

  # register run_tests with CTest
  add_test(NAME UnitTests COMMAND run_tests)
endif()

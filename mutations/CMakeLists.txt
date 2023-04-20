cmake_minimum_required(VERSION 3.16)

project(CWhyMutations)

include(FetchContent)
include(CheckIPOSupported)

set(CMAKE_CXX_STANDARD 20)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)
message(STATUS "Found LLVM/Clang ${LLVM_PACKAGE_VERSION}")

FetchContent_Declare(
    boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG b6928ae5c92e21a04bbe17a558e6e066dbe632f6 # boost-1.82.0
)

message(STATUS "Downloading and configuring Boost")
FetchContent_MakeAvailable(boost)

FetchContent_Declare(
  range-v3
  GIT_REPOSITORY https://github.com/ericniebler/range-v3.git
  GIT_TAG a81477931a8aa2ad025c6bda0609f38e09e4d7ec # 0.12.0
)

message(STATUS "Downloading and configuring Range-v3")
FetchContent_MakeAvailable(range-v3)

add_executable(mutations src/main.cpp)
target_compile_options(mutations PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(mutations SYSTEM PRIVATE ${CLANG_INCLUDE_DIRS})
target_link_libraries(mutations PRIVATE Boost::program_options)
target_link_libraries(mutations PRIVATE clangAST clangASTMatchers clangFrontend clangTooling)
target_link_libraries(mutations PRIVATE range-v3)

check_ipo_supported(RESULT IPO_SUPPORTED)
if(IPO_SUPPORTED)
    message(STATUS "IPO/LTO supported")
    set_property(TARGET mutations PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO/LTO not supported")
endif()

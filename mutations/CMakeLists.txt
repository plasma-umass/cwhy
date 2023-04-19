cmake_minimum_required(VERSION 3.16)

include(FetchContent)

project(CWhyMutations)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost COMPONENTS program_options REQUIRED)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)
message(STATUS "Found LLVM/Clang ${LLVM_PACKAGE_VERSION}.")

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG a33701196adfad74917046096bf5a2aa0ab0bb50 # 9.1.0
)

message(STATUS "Downloading and configuring {fmt}")
FetchContent_MakeAvailable(fmt)

add_executable(mutations src/main.cpp)
target_compile_options(mutations PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(mutations SYSTEM PRIVATE ${CLANG_INCLUDE_DIRS})
target_link_libraries(mutations PRIVATE Boost::program_options)
target_link_libraries(mutations PRIVATE clangAST clangASTMatchers clangFrontend clangTooling)
target_link_libraries(mutations PRIVATE fmt)


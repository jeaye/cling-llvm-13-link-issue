cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cling-demo)

find_package(LLVM REQUIRED CONFIG HINTS "${cling_build_dir}/lib/cmake/llvm")
find_package(
  Clang REQUIRED CONFIG
  HINTS "${cling_build_dir}/tools/clang/lib/cmake/clang"
        "${cling_build_dir}/lib/cmake/clang"
)
find_package(
  Cling REQUIRED CONFIG
  HINTS "${cling_build_dir}/tools/cling/lib/cmake/cling"
        "${cling_build_dir}/lib/cmake/cling"
)
# Only needed for older versions of Cling.
list(APPEND CLING_INCLUDE_DIRS "${cling_build_dir}/../cling-src/tools/cling/include")

add_executable(cling-demo src/main.cpp)
target_include_directories(cling-demo PUBLIC "include")
target_compile_features(cling-demo PUBLIC cxx_std_17)

target_link_libraries(cling-demo clingInterpreter bsd z ncursesw)
target_include_directories(cling-demo PUBLIC ${CLING_INCLUDE_DIRS})
target_include_directories(cling-demo PUBLIC ${CLANG_INCLUDE_DIRS})
target_include_directories(cling-demo PUBLIC ${LLVM_INCLUDE_DIRS})

target_compile_options(cling-demo PUBLIC -DLLVMDIR="${LLVM_BINARY_DIR}")

set_target_properties(cling-demo PROPERTIES ENABLE_EXPORTS 1)

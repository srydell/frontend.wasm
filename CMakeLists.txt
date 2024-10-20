cmake_minimum_required(VERSION 3.25)

# Temporary workaround for clang 14
# https://github.com/llvm/llvm-project/issues/53950
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  set(frontend_languages CXX)
else()
  set(frontend_languages C CXX)
endif()

project(
  Frontend.wasm
  VERSION 0.6.0
  LANGUAGES ${frontend_languages})

configure_file(docs/ReleaseNotes/version.in
               ${CMAKE_CURRENT_LIST_DIR}/docs/ReleaseNotes/version @ONLY)

# Set ccache, ipo, etc.
include(./cmake/ProjectSettings.cmake)

include(./cmake/WarningsAndOptions.cmake)

include(./cmake/Sanitizers.cmake)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

include(FetchContent)
FetchContent_Declare(
  IRSpecification
  GIT_REPOSITORY https://github.com/Tolc-Software/IntermediateRepresentation.git
  GIT_TAG v0.15.1)

FetchContent_MakeAvailable(IRSpecification)

add_library(
  Frontend.wasm
  src/Embind/Builders/attributeBuilder.cpp
  src/Embind/Builders/classBuilder.cpp
  src/Embind/Builders/enumBuilder.cpp
  src/Embind/Builders/functionBuilder.cpp
  src/Embind/Builders/moduleBuilder.cpp
  src/Embind/Builders/moduleFileBuilder.cpp
  src/Embind/Builders/typeToStringBuilder.cpp
  src/Embind/Proxy/attribute.cpp
  src/Embind/Proxy/class.cpp
  src/Embind/Proxy/enum.cpp
  src/Embind/Proxy/function.cpp
  src/Embind/Proxy/module.cpp
  src/Embind/Proxy/moduleFile.cpp
  src/Frontend/Wasm/frontend.cpp
  src/Embind/checkType.cpp
  src/Embind/createFunctions.cpp
  src/Embind/getOverloadedFunctions.cpp
  src/Embind/Helpers/split.cpp
  src/Embind/Helpers/string.cpp
  src/Embind/Helpers/trampolineClass.cpp
  src/Embind/Helpers/types.cpp)

add_warnings(TARGET Frontend.wasm)
add_options(TARGET Frontend.wasm)

# Set the sanitizer options on project_options
enable_sanitizers(Frontend.wasm)

target_link_libraries(
  Frontend.wasm
  PUBLIC IR::IR
  PRIVATE fmt::fmt spdlog::spdlog)

target_include_directories(Frontend.wasm PRIVATE src)
target_include_directories(
  Frontend.wasm PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                       $<INSTALL_INTERFACE:include>)

set_target_properties(Frontend.wasm PROPERTIES CXX_STANDARD_REQUIRED ON
                                               CXX_EXTENSIONS OFF)

# Add library to link against
add_library(Tolc::Frontend.wasm ALIAS Frontend.wasm)

# Only build tests and benchmarks if building this project directly
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(ENABLE_TESTING "Build the testing tree" ON)
  if(ENABLE_TESTING)
    option(ENABLE_COVERAGE "Use code coverage tools on the tests" ON)
    enable_testing()
    add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" ON)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()

  option(ENABLE_PACKAGING "Enables install and packaging via CPack" ON)
  if(ENABLE_PACKAGING)
    add_subdirectory(tools/packaging)
  endif()
endif()

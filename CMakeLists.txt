cmake_minimum_required(VERSION 3.12...3.22)

project(
  "porth-c"
  LANGUAGES "C"
  VERSION "0.1.0"
  DESCRIPTION "Porth in C"
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/PushDirectory")
include(PushDirectory)

configure_file("cmake/config.h.cin" "${PROJECT_BINARY_DIR}/include/config.h")

push_directory_library(
  TARGET_NAME argparse
  SOURCE_DIRECTORY_COMES_LAST
  SOURCE_DIRECTORY source
  SOURCE_FILES argparse.c
  INCLUDED_DIRECTORIES argparse/include
)
push_directory_library(
  TARGET_NAME toolbox
  SOURCE_DIRECTORY_COMES_LAST
  SOURCE_DIRECTORY source
  SOURCE_FILES hash.c
  INCLUDED_DIRECTORIES toolbox/include
)
push_directory_library(
  TARGET_NAME nonstd
  SOURCE_DIRECTORY_COMES_LAST
  SOURCE_DIRECTORY source
  SOURCE_FILES strdup.c strpbrk.c strspn.c strtok.c
  INCLUDED_DIRECTORIES nonstd/include
)
push_directory_library(
  TARGET_NAME iota
  SOURCE_DIRECTORY_COMES_LAST
  SOURCE_DIRECTORY source
  SOURCE_FILES iota.c
  INCLUDED_DIRECTORIES iota/include
  LINKED_LIBRARIES toolbox nonstd
)
push_directory_library(
  TARGET_NAME subprocess
  SOURCE_DIRECTORY_COMES_LAST
  SOURCE_DIRECTORY source
  SOURCE_FILES dummy.c
  INCLUDED_DIRECTORIES subprocess/include
)
push_directory_executable(
  TARGET_NAME porth OUTPUT_NAME porth.exe
  SOURCE_DIRECTORY_COMES_LAST
  SOURCE_DIRECTORY source
  SOURCE_FILES main.c op.c sim.c com.c
  INCLUDED_DIRECTORIES porth/include "${PROJECT_BINARY_DIR}/include"
  LINKED_LIBRARIES iota toolbox subprocess
)
push_directory_executable(
  TARGET_NAME embed
  SOURCE_DIRECTORY_COMES_LAST
  SOURCE_DIRECTORY source
  SOURCE_FILES main.c
  LINKED_LIBRARIES argparse nonstd
)
add_custom_command(
  OUTPUT "${PROJECT_BINARY_DIR}/porth/iota/ops_iota.c"
         "${PROJECT_BINARY_DIR}/porth/iota/ops_iota.h"
  DEPENDS embed
  COMMAND "${CMAKE_COMMAND}" -E make_directory
          "${PROJECT_BINARY_DIR}/porth/iota"
  COMMAND "$<TARGET_FILE:embed>" "${PROJECT_SOURCE_DIR}/porth/iota/ops.iota" -o
          "${PROJECT_BINARY_DIR}/porth/iota/ops_iota.c" -s "ops_iota"
)
target_sources(porth PRIVATE "${PROJECT_BINARY_DIR}/porth/iota/ops_iota.c")
target_include_directories(porth PRIVATE "${PROJECT_BINARY_DIR}/porth/iota")
add_custom_command(
  OUTPUT "${PROJECT_BINARY_DIR}/porth/asmt/main_asmt.c"
         "${PROJECT_BINARY_DIR}/porth/asmt/main_asmt.h"
  DEPENDS embed
  COMMAND "${CMAKE_COMMAND}" -E make_directory
          "${PROJECT_BINARY_DIR}/porth/asmt"
  COMMAND "$<TARGET_FILE:embed>" "${PROJECT_SOURCE_DIR}/porth/asmt/main.asmt"
          -o "${PROJECT_BINARY_DIR}/porth/asmt/main_asmt.c" -s "main_asmt"
)
target_sources(porth PRIVATE "${PROJECT_BINARY_DIR}/porth/asmt/main_asmt.c")
target_include_directories(porth PRIVATE "${PROJECT_BINARY_DIR}/porth/asmt")

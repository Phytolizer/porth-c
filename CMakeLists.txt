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
push_directory_executable(
  TARGET_NAME porth
  SOURCE_DIRECTORY_COMES_LAST
  SOURCE_DIRECTORY source
  SOURCE_FILES main.c
  INCLUDED_DIRECTORIES "${PROJECT_BINARY_DIR}/include"
  LINKED_LIBRARIES iota toolbox
)

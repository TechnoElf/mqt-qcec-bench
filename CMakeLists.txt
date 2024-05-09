cmake_minimum_required(VERSION 3.19...3.28)

project(
  mqt-qcec-bench
  LANGUAGES CXX
  DESCRIPTION "MQT QCEC Bench")

include(cmake/ExternalDependencies.cmake)

add_subdirectory(src)

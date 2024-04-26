# Declare all external dependencies and make sure that they are available.

include(FetchContent)
set(FETCH_PACKAGES "")

set(FETCHCONTENT_SOURCE_DIR_MQT-QCEC
    ${PROJECT_SOURCE_DIR}/extern/mqt-qcec
    CACHE
      PATH
      "Path to the source directory of the mqt-qcec library. This variable is used by FetchContent to download the library if it is not already available."
)
set(MQT_QCEC_VERSION
    2.6.0
    CACHE STRING "MQT QCEC version")
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
  FetchContent_Declare(
    mqt-qcec
    GIT_REPOSITORY https://github.com/cda-tum/mqt-qcec.git
    GIT_TAG v${MQT_QCEC_VERSION}
    FIND_PACKAGE_ARGS ${MQT_QCEC_VERSION})
  list(APPEND FETCH_PACKAGES mqt-qcec)
else()
  find_package(mqt-qcec ${MQT_QCEC_VERSION} QUIET)
  if(NOT mqt-qcec_FOUND)
    FetchContent_Declare(
      mqt-qcec
      GIT_REPOSITORY https://github.com/cda-tum/mqt-qcec.git
      GIT_TAG v${MQT_QCEC_VERSION})
    list(APPEND FETCH_PACKAGES mqt-qcec)
  endif()
endif()

# Make all declared dependencies available.
FetchContent_MakeAvailable(${FETCH_PACKAGES})

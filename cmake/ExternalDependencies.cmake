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

set(TCLAP_VERSION
        1.4.0-rc1
        CACHE STRING "TCLAP version")
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
  FetchContent_Declare(
          tclap
          GIT_REPOSITORY https://git.code.sf.net/p/tclap/code
          GIT_TAG ${TCLAP_VERSION}
          FIND_PACKAGE_ARGS)
  list(APPEND FETCH_PACKAGES tclap)
else()
  find_package(tclap ${TCLAP_VERSION} QUIET)
  if(NOT tclap_FOUND)
    FetchContent_Declare(
            tclap
            GIT_REPOSITORY https://git.code.sf.net/p/tclap/code
            GIT_TAG ${TCLAP_VERSION})
    list(APPEND FETCH_PACKAGES tclap)
  endif()
endif()

set(TOMLPLUSPLUS_VERSION
        v3.4.0
        CACHE STRING "toml++ version")
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
  FetchContent_Declare(
          tomlplusplus
          GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
          GIT_TAG ${TOMLPLUSPLUS_VERSION}
          FIND_PACKAGE_ARGS)
  list(APPEND FETCH_PACKAGES tomlplusplus)
else()
  find_package(tomlplusplus ${TOMLPLUSPLUS_VERSION} QUIET)
  if(NOT tomlplusplus_FOUND)
    FetchContent_Declare(
            tomlplusplus
            GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
            GIT_TAG ${TCLAP_VERSION})
    list(APPEND FETCH_PACKAGES tomlplusplus)
  endif()
endif()

# Make all declared dependencies available.
FetchContent_MakeAvailable(${FETCH_PACKAGES})

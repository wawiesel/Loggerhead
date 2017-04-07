#
# Boilerplate main CMakeLists.txt file for BootsOnTheGround PROJECT.
#
CMAKE_MINIMUM_REQUIRED(VERSION 3.0 FATAL_ERROR)

INCLUDE( "${CMAKE_SOURCE_DIR}/cmake/BOTG_INCLUDE.cmake" )
botgDownloadExternalProjects(
    external/Testing123.in
)

botgProject()

GLOBAL_SET( ${PROJECT_NAME}_ENABLE_SECONDARY_TESTED_CODE OFF )

PROJECT( "${PROJECT_NAME}"
    VERSION "${PROJECT_VERSION}"
    LANGUAGES C CXX Fortran
)

botgEnd()

##
##
##CMakeLists.txt
##------------------------------------------------------------------------------
##This is the main CMakeLists.txt file.
##
##.. code-block:: cmake
##
CMAKE_MINIMUM_REQUIRED(VERSION 3.0 FATAL_ERROR)

# This sets BOTG_SOURCE_DIR which may be used later.
INCLUDE( "${CMAKE_SOURCE_DIR}/cmake/BOTG_INCLUDE.cmake" )

# Download external projects.
BOTG_DownloadExternalProjects( Testing123 )

# Configure the project.
BOTG_ConfigureProject( "${CMAKE_SOURCE_DIR}" )
TRIBITS_PROJECT_ENABLE_ALL()

# Final print of all the variables for inspection.
# For example: -D MATCH_VARIABLE_REGEX:STRING="" will print everything.
#              -D MATCH_VARIABLE_REGEX:STRING="^BootsOnTheGround" will
#                 print all the BootsOnTheGround variables.
#
IF( DEFINED MATCH_VARIABLE_REGEX )
    BOTG_PrintAllVariables("${MATCH_VARIABLE_REGEX}")
ENDIF()

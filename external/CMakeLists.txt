##
##
##external/CMakeLists.txt
##------------------------------------------------------------------------------
##This CMakeLists.txt file drives the download of all the dependent
##repositories. We just need to add them to the ``external_projects``
##list below. It is expected to find a *.in file for each, e.g.
##``Testing123.in``.
##
##.. code-block:: cmake
##

#List of external projects.
SET(external_projects
    Testing123
)

#Loop over them and configure and execute an internal configure/build
#to download each one.
FOREACH( ep ${external_projects} )
    MESSAGE( STATUS "loading external project=${ep}...")
    CONFIGURE_FILE(${ep}.in download/${ep}/CMakeLists.txt)
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
        . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/download/${ep})
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} --build
        . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/download/${ep})
ENDFOREACH()

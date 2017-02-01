SET(external_projects
    Testing123
)

FOREACH( ep ${external_projects} )
    MESSAGE( STATUS "loading external project=${ep}...")
    CONFIGURE_FILE(${ep}.in download/${ep}/CMakeLists.txt)
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
        . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/download/${ep})
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} --build
        . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/download/${ep})
ENDFOREACH()

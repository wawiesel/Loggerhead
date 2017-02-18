##
##
##Version.cmake
##------------------------------------------------------------------------------
##TriBITS requires this file but we aren't really using it...yet.
##
##.. code-block:: cmake
##
IF ("${REPOSITORY_NAME}" STREQUAL "")
  SET(REPOSITORY_NAME "Loggerhead")
ENDIF()
SET(${REPOSITORY_NAME}_MAJOR_VERSION "0" )
SET(${REPOSITORY_NAME}_VERSION "${${REPOSITORY_NAME}_MAJOR_VERSION}.1")


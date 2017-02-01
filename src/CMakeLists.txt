##
##
##src/CMakeLists.txt
##------------------------------------------------------------------------------
##This is a standard **package** CMakeLists.txt file describing
##the code. Below I build an executable and declare a test
##directory. You can also build a library with ``TRIBITS_ADD_LIBRARY``.
##
##.. code-block:: cmake
##

#Do this at the beginning of a package.
TRIBITS_PACKAGE( Template123 )

TRIBITS_ADD_TEST_DIRECTORIES( test )

TRIBITS_ADD_EXECUTABLE( demo
  NOEXEPREFIX
  SOURCES
    demo.cc
)

#Do this at the end.
TRIBITS_PACKAGE_POSTPROCESS()

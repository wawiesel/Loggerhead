##
##
##src/CMakeLists.txt
##------------------------------------------------------------------------------
##This is a standard **package** CMakeLists.txt file describing
##the code. Below I build an executable, a library, install headers,
###and declare a test directory.
##
##.. code-block:: cmake
##

#Do this at the beginning of a package.
TRIBITS_PACKAGE( Template123 )

BOTG_AddCompilerFlags( CXX ANY ANY
    "-std=c++11"
)

TRIBITS_ADD_LIBRARY( Template123
  SOURCES
    spdlog/sinks/progress_sink.cc
    term/color.cc
    term/info.cc
    term/progress_bar.cc
    term/progress_calculator.cc
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_LIST_DIR}
)

#include/spdlog
INSTALL(
  FILES
    spdlog/progress_logger.hh
    spdlog/progress_logger.i.hh
  DESTINATION
    include/spdlog
)

#include/spdlog/sinks
INSTALL(
  FILES
    spdlog/sinks/progress_sink.hh
    spdlog/sinks/progress_sink.i.hh
  DESTINATION
    include/spdlog
)

#include/term
INSTALL(
  FILES
    term/color.hh
    term/info.hh
    term/progress_bar.hh
    term/progress_calculator.hh
  DESTINATION
    include/term
)

TRIBITS_ADD_EXECUTABLE( demo
  NOEXEPREFIX
  SOURCES
    demo.cc
)

TRIBITS_ADD_TEST_DIRECTORIES(
    test
    spdlog/test
    term/test
)

#Do this at the end.
TRIBITS_PACKAGE_POSTPROCESS()

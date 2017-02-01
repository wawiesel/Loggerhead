Template123
===========

This is a template for projects which use

   Testing123 --> BootsOnTheGround --> TriBITS --> CMake

Yes, it is a deep heirarchy.

- Testing123 gives us a unit testing framework in C++ and Fortran.
- BootsOnTheGround gives us third-party library (TPL) management,
  including on-the-fly downloads.
- TriBITS gives us project dependency management and best-practice macros.
- CMake gives us cross-platform (Windows/Linux/Mac) compatibility.

Let's go through the project in the order it is processed by CMake [*]_.

File Tree
---------
    .
    ├── CMakeLists.txt
    ├── LICENSE
    ├── PackagesList.cmake
    ├── ProjectName.cmake
    ├── README.rst
    ├── TPLsList.cmake
    ├── Version.cmake
    ├── external
    │   ├── CMakeLists.txt
    │   └── Testing123.in
    └── src
        ├── CMakeLists.txt
        ├── cmake
        │   └── Dependencies.cmake
        ├── demo.cc
        └── test
            ├── CMakeLists.txt
            ├── tstDemo.cc
            └── tstDemo.f90


CMakeLists.txt
------------------------------------------------------------------------------
This is the main CMakeLists.txt file.

.. code-block:: cmake

    CMAKE_MINIMUM_REQUIRED(VERSION 3.0 FATAL_ERROR)

    # First download external repositories.
    ADD_SUBDIRECTORY( external )

    # This sets BOTG_SOURCE_DIR which may be used later.
    INCLUDE( "${CMAKE_SOURCE_DIR}/external/Testing123/external/BootsOnTheGround/cmake/BOTG_INCLUDE.cmake" )

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


external/CMakeLists.txt
------------------------------------------------------------------------------
This CMakeLists.txt file drives the download of all the dependent
repositories. We just need to add them to the ``external_projects``
list below. It is expected to find a *.in file for each, e.g.
``Testing123.in``.

.. code-block:: cmake


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


external/Testing123.in
------------------------------------------------------------------------------
This file is configured and then treated like its own
CMakeLists.txt file to drive the download using only
CMake and the awesome ``ExternalProject_Add`` command.

.. code-block:: cmake

    CMAKE_MINIMUM_REQUIRED(VERSION 2.8.2)
    PROJECT(download-external-Testing123 NONE)

    INCLUDE(ExternalProject)
    ExternalProject_Add( download-external-Testing123
      GIT_REPOSITORY
        https://github.com/wawiesel/Testing123.git
      GIT_TAG
        master
      SOURCE_DIR
        "${CMAKE_CURRENT_LIST_DIR}/Testing123"
      CONFIGURE_COMMAND ""
      BUILD_COMMAND     ""
      INSTALL_COMMAND   ""
      TEST_COMMAND      ""
    )



PackagesList.cmake
------------------------------------------------------------------------------
This is the list of dependent package files.
Every TriBITS repository/project must define this.
I put the ones in external that I do not own and the ones I do
own in src/. Note that the external packages will only be downloaded
at configure time and we are using Testing123's internal BootsOnTheGround.
Note also that we point to the ``src`` directory because that is where
the **package** CMakeLists.txt resides, not to be confused with the
**project** CMakeLists.txt which sits at the root and provides "standalone"
configure/build capability.

Finally, do note that the order is important! We must move downstream
from most basic to most complex for TriBITS to resolve dependencies.

.. code-block:: cmake

    TRIBITS_REPOSITORY_DEFINE_PACKAGES(
      BootsOnTheGround external/Testing123/external/BootsOnTheGround/src          ST
      Testing123       external/Testing123/src                                    ST
      Template123      src                                                        PT
    )


PackagesList.cmake
------------------------------------------------------------------------------
Declare the **project** name to be something different
than the **package** name! I prefix it with ``project-``.

.. code-block:: cmake


    SET(PROJECT_NAME project-Template123)


Version.cmake
------------------------------------------------------------------------------
TriBITS requires this file but we aren't really using it...yet.

.. code-block:: cmake

    IF ("${REPOSITORY_NAME}" STREQUAL "")
      SET(REPOSITORY_NAME "Template123")
    ENDIF()
    SET(${REPOSITORY_NAME}_MAJOR_VERSION "0" )
    SET(${REPOSITORY_NAME}_VERSION "${${REPOSITORY_NAME}_MAJOR_VERSION}.1")


src/CMakeLists.txt
------------------------------------------------------------------------------
This is a standard **package** CMakeLists.txt file describing
the code. Below I build an executable and declare a test
directory. You can also build a library with ``TRIBITS_ADD_LIBRARY``.

.. code-block:: cmake


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

src/CMakeLists.txt
------------------------------------------------------------------------------
Every TriBITS package must declare its dependencies.
If you are using BootsOnTheGround, then you can add TPLs
**after** ``TRIBITS_PACKAGE_DEFINE_DEPENDENCIES``.

.. code-block:: cmake


    #Depend on Testing123 only for testing, but have TPL Spdlog
    #available everywhere.
    TRIBITS_PACKAGE_DEFINE_DEPENDENCIES(
      TEST_REQUIRED_PACKAGES
        Testing123
    )
    BOTG_AddTPL( LIB REQUIRED Spdlog )

Regeneration Script
-------------------

.. code-block:: bash

    for f in CMakeLists.txt \
             external/CMakeLists.txt \
             external/Testing123.in \
             PackagesList.cmake \
             ProjectName.cmake \
             Version.cmake \
             src/CMakeLists.txt \
             src/cmake/Dependencies.cmake;
    do
        gsed 's|^|    |g' $f | gsed 's|^    ##||g'
    done

.. [*] These sections may be regenerated by running the script in `Regeneration Script`_.

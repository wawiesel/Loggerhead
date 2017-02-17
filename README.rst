Loggerhead [WIP]
================

CMake/TriBITS-enabled C++/Fortran logging and more using Spdlog

.. image:: http://i.imgur.com/YAQaRZx.jpg

This has been migrated from a project template called Templat123 
demonstrating the use of Testing123_ for unit testing.

::

   -->Testing123
      -->BootsOnTheGround
         -->TriBITS
            -->CMake

Yes, it is a deep heirarchy.

- Testing123_ gives us a unit testing framework in C++ and Fortran.
- BootsOnTheGround (BOTG_) gives us third-party library (TPL) management,
  including on-the-fly downloads.
- TriBITS_ gives us project dependency management and best-practice macros.
- CMake_ gives us cross-platform (Windows/Linux/Mac) compatibility.

Let's go through the Template123_ project like ``cmake`` [*]_.

Template123 File Tree
---------------------

::

    .
    ├── CMakeLists.txt
    ├── LICENSE
    ├── PackagesList.cmake
    ├── ProjectName.cmake
    ├── README.rst
    ├── TPLsList.cmake
    ├── Version.cmake
    ├── cmake
    │   └── BOTG_INCLUDE.cmake
    ├── external
    │   ├── BootsOnTheGround.in
    │   └── Testing123.in
    └── src
        ├── CMakeLists.txt
        ├── cmake
        │   └── Dependencies.cmake
        ├── demo.cc
        ├── spdlog
        │   ├── progress_logger.hh
        │   ├── progress_logger.i.hh
        │   ├── sinks
        │   │   ├── progress_sink.cc
        │   │   ├── progress_sink.hh
        │   │   └── progress_sink.i.hh
        │   └── test
        │       ├── CMakeLists.txt
        │       └── tstprogress_logger.cc
        ├── term
        │   ├── color.cc
        │   ├── color.hh
        │   ├── info.cc
        │   ├── info.hh
        │   ├── progress_bar.cc
        │   ├── progress_bar.hh
        │   ├── progress_calculator.cc
        │   ├── progress_calculator.hh
        │   └── test
        │       ├── CMakeLists.txt
        │       ├── tstcolor.cc
        │       ├── tstinfo.cc
        │       ├── tstprogress_bar.cc
        │       └── tstprogress_calculator.cc
        └── test
            ├── CMakeLists.txt
            ├── tstDemo.cc
            └── tstDemo.f90


cmake/BOTG_INCLUDE.cmake
------------------------------------------------------------------------------
This file is the key file to include from a project's central
CMakeLists.txt to perform the BootsOnTheGround magic. It even
bootstraps in a copy of the BootsOnTheGround repo if not found.

.. code-block:: cmake


    #This function needs to be here instead of in BOTG.cmake so it can be part of a
    #Bootstrapping operation.
    MACRO( BOTG_DownloadExternalProjects external_projects )
        FOREACH( ep ${external_projects} )
            IF( NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ep}" )
                MESSAGE( FATAL_ERROR "[BootsOnTheGround] cannot find external project download file=${ep}" )
            ENDIF()
            GET_FILENAME_COMPONENT( dir "${ep}" DIRECTORY )
            GET_FILENAME_COMPONENT( project "${ep}" NAME_WE )
            SET( BOTG_EXTERNAL_SOURCE_DIR_${project} "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/${project}" CACHE PATH INTERNAL)

            SET( dest "${CMAKE_BINARY_DIR}/_DOWNLOAD/${dir}/${project}" )
            MESSAGE( STATUS "[BootsOnTheGround] bootstrapping project ${ep} ... ")

            CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/${ep}" "${dest}/CMakeLists.txt" )
            EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" . WORKING_DIRECTORY "${dest}")
            EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY "${dest}")
        ENDFOREACH()
    ENDMACRO()


    # Default to bootstrapping.
    SET(BOTG_BOOTSTRAP ON CACHE BOOL INTERNAL)

    # If we bootstrap, then set the source directory and update the projects.
    IF( BOTG_BOOTSTRAP )
        SET(BOTG_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/BootsOnTheGround" CACHE PATH INTERNAL)
        BOTG_DownloadExternalProjects(
            external/BootsOnTheGround.in
        )
    #If we don't, then we are building BootsOnTheGround as a project so it's easy.
    ELSE()
        SET(BOTG_SOURCE_DIR "${CMAKE_SOURCE_DIR}" CACHE PATH INTERNAL)
    ENDIF()

    MESSAGE( STATUS "[BootsOnTheGround] using BOTG_SOURCE_DIR=${BOTG_SOURCE_DIR}")

    # Includes all the "BootsOnTheGround" (BOTG) functions.
    INCLUDE( "${BOTG_SOURCE_DIR}/cmake/BOTG.cmake" )




CMakeLists.txt
------------------------------------------------------------------------------
This is the main CMakeLists.txt file.

.. code-block:: cmake

    CMAKE_MINIMUM_REQUIRED(VERSION 3.0 FATAL_ERROR)

    # This sets BOTG_SOURCE_DIR which may be used later.
    INCLUDE( "${CMAKE_SOURCE_DIR}/cmake/BOTG_INCLUDE.cmake" )

    # Download external projects.
    BOTG_DownloadExternalProjects(
        external/Testing123.in
    )

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


PackagesList.cmake
------------------------------------------------------------------------------
This is the list of dependent package files.
Every TriBITS repository/project must define this.
I put the ones in external that I do not own and the ones I do
own in src/. Note that the external packages will only be downloaded
at configure time. Note also that we point to the ``src`` directory because
that is where the **package** CMakeLists.txt resides, not to be confused
with the **project** CMakeLists.txt which sits at the root and provides
"standalone" configure/build capability.

Finally, do note that the order is important! We must move downstream
from most basic to most complex for TriBITS to resolve dependencies.

.. code-block:: cmake

    TRIBITS_REPOSITORY_DEFINE_PACKAGES(
      BootsOnTheGround external/BootsOnTheGround/src     ST
      Testing123       external/Testing123/src           ST
      Template123      src                               PT
    )


PackagesList.cmake
------------------------------------------------------------------------------
Declare the **project** name to be something different
than the **package** name! I prefix it with ``project-``.

.. code-block:: cmake


    SET(PROJECT_NAME project-Template123)


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


src/CMakeLists.txt
------------------------------------------------------------------------------
This is a standard **package** CMakeLists.txt file describing
the code. Below I build an executable, a library, install headers,
#and declare a test directory.

.. code-block:: cmake


    #Do this at the beginning of a package.
    TRIBITS_PACKAGE( Template123 )

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


src/spdlog/test/CMakeLists.txt
------------------------------------------------------------------------------
This is usually all that's required in a unit test CMakeLists.txt file.

.. code-block:: cmake


    ADD_CXX_TEST( tstprogress_logger.cc )


src/term/test/CMakeLists.txt
------------------------------------------------------------------------------
This is usually all that's required in a unit test CMakeLists.txt file.

.. code-block:: cmake


    ADD_CXX_TEST( tstprogress_calculator.cc )
    ADD_CXX_TEST( tstprogress_bar.cc )
    ADD_CXX_TEST( tstcolor.cc )
    ADD_CXX_TEST( tstinfo.cc )


src/test/CMakeLists.txt
------------------------------------------------------------------------------
This is usually all that's required in a unit test CMakeLists.txt file.

.. code-block:: cmake

    ADD_FORTRAN_TEST( tstDemo.f90 )
    ADD_CXX_TEST( tstDemo.cc )


TPLsList.cmake
------------------------------------------------------------------------------
This is a required TriBITS file and here we just defer to the one
in BootsOnTheGround. If you are using BootsOnTheGround, this is what
you should do. If BootsOnTheGround doesn't have your TPL...add it!

.. code-block:: cmake

    INCLUDE( "${CMAKE_SOURCE_DIR}/external/BootsOnTheGround/TPLsList.cmake" )


Version.cmake
------------------------------------------------------------------------------
TriBITS requires this file but we aren't really using it...yet.

.. code-block:: cmake

    IF ("${REPOSITORY_NAME}" STREQUAL "")
      SET(REPOSITORY_NAME "Template123")
    ENDIF()
    SET(${REPOSITORY_NAME}_MAJOR_VERSION "0" )
    SET(${REPOSITORY_NAME}_VERSION "${${REPOSITORY_NAME}_MAJOR_VERSION}.1")



Regeneration Script
-------------------

.. code-block:: bash

    for f in \
        cmake/BOTG_INCLUDE.cmake\
        CMakeLists.txt\
        PackagesList.cmake\
        ProjectName.cmake\
        src/cmake/Dependencies.cmake\
        src/CMakeLists.txt\
        src/spdlog/test/CMakeLists.txt\
        src/term/test/CMakeLists.txt\
        src/test/CMakeLists.txt\
        TPLsList.cmake\
        Version.cmake;
    do
        gsed 's|^|    |g' $f | gsed 's|^    ##||g'
    done

.. [*] These sections may be regenerated by running the script in `Regeneration Script`_.
.. _CMake: https://cmake.org/
.. _TriBITS: https://tribits.org
.. _BOTG: http://github.com/wawiesel/BootsOnTheGround
.. _Testing123: http://github.com/wawiesel/Testing123
.. _Template123: http://github.com/wawiesel/Template123




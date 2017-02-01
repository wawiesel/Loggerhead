Template123
===========

This is a template for projects which use

   Testing123 -->
      BootsOnTheGround -->
         TriBITS -->
            CMake

Yes, it is a deep heirarchy.

- Testing123 gives us a unit testing framework in C++ and Fortran.
- BootsOnTheGround gives us third-party library (TPL) management,
  including on-the-fly downloads.
- TriBITS gives us project dependency management and best-practice macros.
- CMake gives us cross-platform (Windows/Linux/Mac) compatibility.

Let's go through the template in the order it is processed

.. literalinclude:: CMakeLists.txt
   :language: cmake

.. literalinclude:: external/CMakeLists.txt
   :language: cmake

.. literalinclude:: external/Testing123.in
   :language: cmake

.. literalinclude:: PackagesList.cmake
   :language: cmake

.. literalinclude:: PackageName.cmake
   :language: cmake

.. literalinclude:: Version.cmake
   :language: cmake

.. literalinclude:: src/CMakeLists.txt
   :language: cmake

.. literalinclude:: src/cmake/Dependencies.cmake
   :language: cmake


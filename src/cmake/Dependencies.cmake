##
##
##src/CMakeLists.txt
##------------------------------------------------------------------------------
##Every TriBITS package must declare its dependencies.
##If you are using BootsOnTheGround, then you can add TPLs
##**after** ``TRIBITS_PACKAGE_DEFINE_DEPENDENCIES``.
##
##.. code-block:: cmake
##

#Depend on Testing123 only for testing, but have TPL Spdlog
#available everywhere.
TRIBITS_PACKAGE_DEFINE_DEPENDENCIES(
  TEST_REQUIRED_PACKAGES
    Testing123
)
BOTG_AddTPL( LIB REQUIRED SPDLOG )

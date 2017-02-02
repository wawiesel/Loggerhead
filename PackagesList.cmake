##
##
##PackagesList.cmake
##------------------------------------------------------------------------------
##This is the list of dependent package files.
##Every TriBITS repository/project must define this.
##I put the ones in external that I do not own and the ones I do
##own in src/. Note that the external packages will only be downloaded
##at configure time. Note also that we point to the ``src`` directory because
##that is where the **package** CMakeLists.txt resides, not to be confused
##with the **project** CMakeLists.txt which sits at the root and provides
##"standalone" configure/build capability.
##
##Finally, do note that the order is important! We must move downstream
##from most basic to most complex for TriBITS to resolve dependencies.
##
##.. code-block:: cmake
##
TRIBITS_REPOSITORY_DEFINE_PACKAGES(
  BootsOnTheGround external/BootsOnTheGround/src                              ST
  Testing123       external/Testing123/src                                    ST
  Template123      src                                                        PT
)

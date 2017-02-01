#Do this at the beginning of a package CMakeList.txt file.
TRIBITS_PACKAGE( Template123 )

#The middle is up to you!

#Here I add a test directory
TRIBITS_ADD_TEST_DIRECTORIES( test )

#and an executable.
TRIBITS_ADD_EXECUTABLE( demo
  NOEXEPREFIX
  SOURCES
    demo.cc
)

#Then I finish it off with this.
TRIBITS_PACKAGE_POSTPROCESS()

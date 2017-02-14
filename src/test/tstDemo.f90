module tstDemo_M
#include "t123/f/TestExe.inc.f90"
implicit none
contains

TEST( A,B ){
    EXPECT_EQ(1,1)
}

end module

program main
use tstDemo_M
!
RUN_ALL_TESTS()
!
end program

#This is the list of dependent package files.
#Every repository/project must define this.
#I put the ones in external that I do not own and the ones I do
#own in src/.
TRIBITS_REPOSITORY_DEFINE_PACKAGES(
  BootsOnTheGround external/Testing123/external/BootsOnTheGround/src          ST
  Testing123       external/Testing123/src                                    ST
  Template123      src                                                        PT
)

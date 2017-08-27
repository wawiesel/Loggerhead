#include <chrono>
#include <fstream>
#include <thread>

#include "gtest/gtest.h"
#include "term/info.hh"

namespace term
{

TEST( info, Basic )
{
    info i(std::cerr);
    EXPECT_LE( 0, i.columns() );
    EXPECT_LE( 0, i.rows() );
    bool is = i.is_tty();
    (void)is; //to silence usage warning
    auto& os = i.os();
}

}//term

#include <chrono>
#include <fstream>
#include <thread>

#include "gtest/gtest.h"
#include "term/progress_bar.hh"

namespace term
{
TEST( progress_bar, Basic )
{
    {
        std::stringstream os;
        progress_bar pb( os );
        EXPECT_FALSE( pb.colorize() );
    }

    {
        std::stringstream os;
        progress_bar pb( os );
        pb.output( 1.0, 120.0, 1.0 );
        EXPECT_EQ(
            " 100% [||||||||||||||||||||||||||||||||||||||||]  2.0m elapsed ( "
            "1.0s left)     ",
            os.str() );
    }

    {
        std::stringstream os;
        progress_bar pb( os );
        pb.output( 0.5, 83.333, 83.333 );
        EXPECT_EQ(
            "  50% [||||||||||||||||||||                    ] 83.3s elapsed "
            "(83.3s left)     ",
            os.str() );
    }
}


void demo_runner(progress_bar& pb)
{
    size_t n = 10; //number of steps
    int dt = 20; //milliseconds
    auto& os = pb.os();
    for( size_t i = 0; i <= n; ++i )
    {
        os << "\r";
        std::this_thread::sleep_for( std::chrono::milliseconds( dt ) );
        double frac = ( 0.0 + i ) / ( 0.0 + n );
        //back to seconds
        double elapsed = i * dt / 1000.;
        double remaining = ( n - i ) * dt / 1000.;
        pb.output( frac, elapsed, remaining );
    }
    os << "\n";
}

TEST( progress_bar, Demo )
{
    std::ostream& os = std::cerr;

    //run with default settings
    progress_bar pb( os );
    demo_runner( pb );

    //change settings and rerun
    pb.set_left_bookend( '|' )
       .set_right_bookend( '>' )
       .set_empty_char('-')
       .set_fill_char('+')
       .set_colorize( color::AUTO )
       .set_empty_color( RED )
       .set_fill_color( GREEN )
       .set_minimum_bar_width( 15 )
       .set_width( 70 )
       .set_bar_width_fraction( 0.6 );
    demo_runner( pb );
}

}//term

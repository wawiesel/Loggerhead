#include <chrono>
#include <fstream>
#include <thread>

#include "gtest/gtest.h"
#include "term/color.hh"

namespace term
{
TEST( color, colorable )
{
    EXPECT_EQ( color::colorable( std::cerr ) , color::colorable( std::cerr, color::AUTO ) );
    EXPECT_FALSE( color::colorable( std::cerr, color::OFF ) );
    EXPECT_TRUE( color::colorable( std::cerr, color::ON ) );

    EXPECT_EQ( color::colorable( std::cout ) , color::colorable( std::cout, color::AUTO ) );
    EXPECT_FALSE( color::colorable( std::cout, color::OFF ) );
    EXPECT_TRUE( color::colorable( std::cout, color::ON ) );

    std::ofstream os( "ofstream.txt" );
    EXPECT_FALSE( color::colorable( os ) );
    EXPECT_FALSE( color::colorable( os, color::AUTO ) );
    EXPECT_FALSE( color::colorable( os, color::OFF ) );
    EXPECT_TRUE( color::colorable( os, color::ON ) );

    std::stringstream ss( "sstream.txt" );
    EXPECT_FALSE( color::colorable( ss ) );
    EXPECT_FALSE( color::colorable( ss, color::AUTO ) );
    EXPECT_FALSE( color::colorable( ss, color::OFF ) );
    EXPECT_TRUE( color::colorable( ss, color::ON ) );
}

// I only know how to do these in Linux.
#if defined( _WIN32 ) || defined( _WIN64 )
#define EXPECT_COLOR_CODE( REF, TEST ) EXPECT_TRUE( true )
#else
#define EXPECT_COLOR_CODE( REF, TEST ) \
    do                                 \
    {                                  \
        std::stringstream ss;          \
        ss << TEST;                    \
        EXPECT_EQ( REF, ss.str() );    \
    } while( false )
#endif

TEST( color, LinuxCodes )
{
    // By setting DEFAULT_BEHAVIOR, we can get term::color to print
    // color codes no matter what.
    {
        std::stringstream ss;
        color::DEFAULT_BEHAVIOR = color::ON;
        EXPECT_TRUE( color::colorable( ss ) );
    }

    EXPECT_COLOR_CODE( "\x1B[31m", color( RED ) );
    EXPECT_COLOR_CODE( "\x1B[37m", color( WHITE ) );
    EXPECT_COLOR_CODE( "\x1B[34m", color( BLUE ) );
    EXPECT_COLOR_CODE( "\x1B[34m\x1B[41m", color( BLUE, RED ) );
    EXPECT_COLOR_CODE( "\x1B[31m\x1B[44m", color( RED, BLUE ) );
    EXPECT_COLOR_CODE( "\x1B[42m", color( NO_CHANGE, GREEN ) );
    EXPECT_COLOR_CODE( "\x1B[33m", color( YELLOW, NO_CHANGE ) );
    EXPECT_COLOR_CODE( "\x1B[33m\x1B[40m", color( YELLOW, BLACK ) );
    EXPECT_COLOR_CODE( "\x1B[35m", color( MAGENTA ) );
    EXPECT_COLOR_CODE( "\x1B[36m", color( CYAN ) );
    EXPECT_COLOR_CODE( "\x1B[37m", color( WHITE ) );
}
}

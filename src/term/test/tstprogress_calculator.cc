#include "gtest/gtest.h"
#include "term/progress_calculator.hh"

#include <chrono>
#include <fstream>
#include <thread>

namespace term
{

TEST( progress_calculator, Initialization )
{
    progress_calculator pc( 100 );
    EXPECT_EQ( 100, pc.total_tasks() );
    EXPECT_EQ( 0, pc.completed_tasks() );
    EXPECT_EQ( 100, pc.remaining_tasks() );
    EXPECT_FALSE( pc.is_finished() );
    EXPECT_LT( 0., pc.elapsed_seconds() );
    EXPECT_EQ( 0., pc.fraction_completed() );
}

TEST( progress_calculator, Update )
{
    progress_calculator pc( 10 );

    std::this_thread::sleep_for( std::chrono::milliseconds(10) );
    pc.update( 1 );
    EXPECT_EQ( 1, pc.completed_tasks() );
    auto e0 = pc.elapsed_seconds();
    auto r0 = pc.remaining_seconds();

    std::this_thread::sleep_for( std::chrono::milliseconds(10) );
    pc.update( 3 );
    EXPECT_EQ( 3, pc.completed_tasks() );
    auto e1 = pc.elapsed_seconds();
    auto r1 = pc.remaining_seconds();

    std::this_thread::sleep_for( std::chrono::milliseconds(10) );
    pc.update( -2 );
    EXPECT_EQ( 5, pc.completed_tasks() );
    auto e2 = pc.elapsed_seconds();
    auto r2 = pc.remaining_seconds();

    pc.finish();

    EXPECT_EQ( 10, pc.completed_tasks() );
    EXPECT_GT( 1.0 , pc.elapsed_seconds() );
    EXPECT_EQ( 0.0 , pc.remaining_seconds() );
    EXPECT_LT( e0 , e1 );
    EXPECT_LT( e1 , e2 );
    EXPECT_GT( r0 , r1 );
    EXPECT_GT( r1 , r2 );

}

}  // term

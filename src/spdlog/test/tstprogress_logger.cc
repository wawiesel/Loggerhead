#include "gtest/gtest.h"
#include "spdlog/progress_logger.hh"

namespace spdlog
{

TEST( progress_logger, Basic ) {
    size_t n = 6;
    auto progress = progress_logger_mt("demo",n);
    //sink must be same threading level as progress_logger
    auto sink = sinks::progress_sink_mt::instance();

    //turn on coloring
    sink->bar.set_colorize(term::color::ON);

    //--------------------------------------------------------------------------
    for(size_t j=1; j<=3; ++j)
    {
        //reset the number of tasks
        sink->reset(n);
        //do not update every time a progress->info,warn,etc. is called
        sink->set_allow_autoupdate(false);
        //try to get a bar 20%,40%,60% of total screen width
        sink->bar.set_bar_width_fraction(0.2*j);
        for( size_t i=1; i<=n; ++i)
        {
            //update i tasks
            sink->update(i);
            //simulate 100 ms
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            //output information about the task
            progress->info( "Completed task {}",i );
        }
        //finish all
        sink->finish();
        //go to next line
        std::cerr << "\n";
    }
    //--------------------------------------------------------------------------
}

void sleep_ms( int ms )
{
    std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
}

TEST( progress_logger, Extra )
{

    //create a multi-threaded progress logger
    size_t n = 8;
    auto progress = progress_logger_mt("extra",n);

    //do a calculation loop
    progress->warn( "Entering a calculation!" );
    for(auto i=1; i<=n-2; ++i)
    {
        sleep_ms( 500 );
        progress->info( "Completed task {}",i );

        //throw an error at task3
        if( i==3 )progress->error("YOU FAIL!");
    }
    sleep_ms( 1000 );
    progress->warn( "Whew! Calculation is over." );

    //go to next line
    std::cerr << "\n";
}

}//spdlog

#include "gtest/gtest.h"
#include "spdlog/progress_logger.hh"

namespace spdlog
{

TEST( progress_logger, Basic ) {
    size_t n = 100;
    auto progress = progress_logger_st("demo",n);
    for( size_t i=1; i<=n; ++i)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
        progress->info( "Completed task {}",i );
    }
}

}//spdlog

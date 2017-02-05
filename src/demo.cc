#include <spdlog/fmt/ostr.h>  // must be included
#include "spdlog/spdlog.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <memory>

namespace spd = spdlog;

// user defined types logging by implementing operator<<
struct my_type
{
    int i;
    template <typename OStream>
    friend OStream& operator<<( OStream& os, const my_type& c )
    {
        os << std::string( c.i, 'x' ) << std::flush;
        system( "sleep 0.05s" );
        return os;
    }
};

int main( int, char* [] )
{
    // Console logger with color
    auto stdout = spd::stdout_logger_mt( "stdout", /*color=*/true );
    stdout->info( "Welcome to spdlog!" );
    stdout->error( "Some error message with arg{}..", 1 );
    stdout->set_level( spdlog::level::trace );
    stdout->warn( "Easy padding in numbers like {:08d}", 12 );
    stdout->error(
        "Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42 );
    stdout->info( "Support for floats {:03.2f}", 1.23456 );
    stdout->info( "Positional args are {1} {0}..", "too", "supported" );
    stdout->info( "{:<30}", "left aligned" );
    stdout->trace( "trace me {:03d}", 7 );

    auto progress = spd::stderr_logger_mt( "stderr", /*color=*/true );
    // auto progress = spd::progress_logger_mt( "progress" );
    for( int i = 0; i < 100; ++i )
    {
        progress->warn( "{:<130}", fmt::format( "{}", i ) );
        std::cerr << my_type{i};
    }
    std::cerr << "\r" << my_type{100} << std::endl;
    return 0;
}

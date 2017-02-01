#include "spdlog/spdlog.h"

#include <iostream>
#include <memory>

namespace spd = spdlog;

int main(int, char*[])
{
    // Console logger with color
    auto stdout = spd::stdout_logger_mt( "stdout", /*color=*/true );
    stdout->info( "Welcome to spdlog!" );
    stdout->error( "Some error message with arg{}..", 1 );
    stdout->set_level( spdlog::level::trace );
    stdout->warn( "Easy padding in numbers like {:08d}", 12 );
    stdout->error(
        "Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}",
        42 );
    stdout->info( "Support for floats {:03.2f}", 1.23456 );
    stdout->info( "Positional args are {1} {0}..", "too", "supported" );
    stdout->info( "{:<30}", "left aligned" );
    stdout->trace( "trace me {:03d}", 7 );
    auto stderr = spd::stderr_logger_mt( "stderr", /*color=*/true );
    stderr->warn( "done" );
    return 0;
}

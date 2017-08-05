#include "info.hh"

#include <sys/ioctl.h>
#include <unistd.h>
#include <cassert>
#include <cstdio>
#include <iostream>

namespace term
{
namespace detail
{
namespace
{
// get whether a isa terminal is attached to the output stream
void getValid( std::ostream& os, bool* isa )
{
    // get handles
    FILE* fh;
    if( &os == &std::cout )
    {
        fh = stdout;
    }
    else if( ( &os == &std::cerr ) || ( &os == &std::clog ) )
    {
        fh = stderr;
    }
    else
    {
        *isa = false;
        return;
    }
#if defined( _WIN32 ) || defined( _WIN64 )
    *isa = ::_isatty( _fileno( fh ) );
#else
    *isa = ::isatty( fileno( fh ) );
#endif

}  // getValid

void getSizes( std::ostream& os, int* columns, int* rows )
{
#if defined( _WIN32 ) || defined( _WIN64 )
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
    *columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w;
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
    *columns = w.ws_col;
    *rows = w.ws_row;
#endif

}  // getSizes

}  // anon
}  // detail

info::info( std::ostream& os )
    : b_os( os ), b_is_tty( false ), b_columns( 0 ), b_rows( 0 )
{
    detail::getValid( b_os, &b_is_tty );
    if( b_is_tty )
    {
        detail::getSizes( b_os, &b_columns, &b_rows );
    }
}

bool info::is_tty() const { return b_is_tty; }
int info::columns() const { return b_columns; }
int info::rows() const { return b_rows; }
std::ostream& info::os()
{
    return b_os;
}
}

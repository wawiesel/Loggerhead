#if defined( _WIN32 ) || defined( _WIN64 )
#define COLOR_WINDOWS
#elif defined( __APPLE__ ) || defined( __unix__ ) || defined( __unix )
#define COLOR_NIX
#else
#error only Windows or *Nix are supported
#endif

#if defined( COLOR_NIX )
#include <unistd.h>
#elif defined( COLOR_WINDOWS )
#include <io.h>
#include <windows.h>
#endif

#include <cstdio>
#include "color.hh"
#include "term/info.hh"

namespace term
{
color::Enum color::DEFAULT_BEHAVIOR = color::AUTO;

// detail namespace for implementation details that depend on OS
namespace detail
{
namespace
{
#if defined( COLOR_NIX )

void modifyColor( std::ostream& os, int fg, int bg )
{
    if( fg == NO_CHANGE && bg == NO_CHANGE )
    {
        os << "\033[00m";
    }
    else
    {
        if( fg != NO_CHANGE ) os << "\033[3" << fg << "m";
        if( bg != NO_CHANGE ) os << "\033[4" << bg << "m";
    }
}

#elif defined( COLOR_WINDOWS )

int convertWindowsColors( int c )
{
    switch( c )
    {
        case BLACK:
            return 0;
        case RED:
            return FOREGROUND_RED;
        case GREEN:
            return FOREGROUND_GREEN;
        case YELLOW:
            return FOREGROUND_GREEN | FOREGROUND_RED;
        case BLUE:
            return FOREGROUND_BLUE;
        case MAGENTA:
            return FOREGROUND_BLUE | FOREGROUND_RED;
        case CYAN:
            return FOREGROUND_BLUE | FOREGROUND_GREEN;
        case WHITE:
            return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
        case default:
            return NO_CHANGE:
    }
}

void modifyColor( std::ostream& os, int fg, int bg )
{
    fg = convertWindowsColors( fg );
    bg = convertWindowsColors( bg );

    static WORD defaultAttributes = 0;

    HANDLE hTerminal = INVALID_HANDLE_VALUE;
    if( &os == &std::cout )
        hTerminal = GetStdHandle( STD_OUTPUT_HANDLE );
    else if( &os == &std::cerr )
        hTerminal = GetStdHandle( STD_ERROR_HANDLE );

    if( !defaultAttributes )
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if( !GetConsoleScreenBufferInfo( hTerminal, &info ) ) return;
        defaultAttributes = info.wAttributes;
    }

    if( fg == NO_CHANGE && bg == NO_CHANGE )
    {
        SetConsoleTextAttribute( hTerminal, defaultAttributes );
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO info;
    if( !GetConsoleScreenBufferInfo( hTerminal, &info ) ) return;

    if( fg != NO_CHANGE )
    {
        info.wAttributes &= ~( info.wAttributes & 0x0F );
        info.wAttributes |= static_cast<WORD>( fg );
    }

    if( bg != NO_CHANGE )
    {
        info.wAttributes &= ~( info.wAttributes & 0xF0 );
        info.wAttributes |= static_cast<WORD>( bg );
    }

    SetConsoleTextAttribute( hTerminal, info.wAttributes );
}

#endif

}  // anonymous namespace
}  // namespace detail

color::color( int fg, int bg ) : b_fg( fg ), b_bg( bg ) {}
color color::reverse() const { return color( b_bg, b_fg ); }
int color::fg() { return b_fg; }
int color::bg() { return b_bg; }
// static
bool color::colorable( std::ostream& os, Enum e )
{
    if( e == color::AUTO ) return info( os ).is_tty();
    return e == color::ON;
}

// static
std::ostream& color::clear( std::ostream& os )
{
    if( colorable( os, DEFAULT_BEHAVIOR ) )
        detail::modifyColor( os, NO_CHANGE, NO_CHANGE );
    return os;
}

// static
std::ostream& color::endl( std::ostream& os )
{
    if( colorable( os, DEFAULT_BEHAVIOR ) )
        detail::modifyColor( os, NO_CHANGE, NO_CHANGE );
    os << std::endl;
    return os;
}

std::ostream& operator<<( std::ostream& os, const color& c )
{
    if( color::colorable( os, color::DEFAULT_BEHAVIOR ) )
        detail::modifyColor( os, c.b_fg, c.b_bg );
    return os;
}

}  // namespace term

#undef COLOR_WINDOWS
#undef COLOR_NIX

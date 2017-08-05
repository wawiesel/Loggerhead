#include "progress_bar.hh"
#include <cassert>
#include <iomanip>
#include <sstream>

namespace term
{
progress_bar::progress_bar( std::ostream& os ) { reset( os ); }
namespace detail
{
namespace
{
// make sure low<=value<=high
template <typename T>
void bound( T low, T& value, T high )
{
    if( value < low ) value = low;
    if( value > high ) value = high;
}
}
}

void progress_bar::reset( std::ostream& os )
{
    b_info = ::term::info( os );
    b_empty_char = ' ';
    b_empty_color = BLUE;
    b_fill_char = '|';
    b_fill_color = MAGENTA;
    b_left_bookend = '[';
    b_right_bookend = ']';
    b_minimum_bar_width = 10;
    b_bar_width_fraction = 0.5;
    b_width = b_info.is_tty() ? b_info.columns() : 80;
    set_colorize( color::AUTO );
}

std::ostream& progress_bar::os()
{
    return b_info.os();
}

progress_bar& progress_bar::set_colorize( color::Enum e )
{
    b_colorize = color::colorable( b_info.os(), e );
    return *this;
}

bool progress_bar::colorize() const { return b_colorize; }
// static
std::string progress_bar::percentage_text( double frac )
{
    std::stringstream ss;
    ss << std::fixed << std::setw( 4 ) << std::setprecision( 0 ) << 100 * frac
       << "% ";
    return ss.str();
}

// static
std::string progress_bar::time_text( double seconds )
{
    std::string unit = "s";
    double time = seconds;
    if( seconds > 5400 )
    {
        unit = "h";
        time /= 3600;
    }
    else if( seconds > 90 )
    {
        unit = "m";
        time /= 60.;
    }
    std::stringstream ss;
    ss << std::fixed << std::setw( 4 ) << std::setprecision( 1 ) << time
       << unit;
    return ss.str();
}

// static
std::string progress_bar::elapsed_text( double seconds )
{
    if( seconds < 0 ) seconds = 0;
    return " " + time_text( seconds ) + " elapsed";
}

// static
std::string progress_bar::remaining_text( double seconds )
{
    if( seconds <= 0 ) return "";
    return " (" + time_text( seconds ) + " left)";
}

// static
std::string progress_bar::bar_text( int length, char c )
{
    std::string s;
    if( length > 0 ) s += std::string( length, c );
    return s;
}

void progress_bar::output( double progress_fraction,
                           double elapsed_seconds,
                           double remaining_seconds )
{
    detail::bound( 0.0, progress_fraction, 1.0 );

    //
    // get text sizes first
    //
    // percentage text:
    std::string pct_text = percentage_text( progress_fraction );

    // elapsed time text:
    std::string ela_text;
    if( elapsed_seconds >= 0.0 ) ela_text = elapsed_text( elapsed_seconds );

    // remaining time text:
    std::string rem_text;
    if( remaining_seconds >= 0.0 )
        rem_text = remaining_text( remaining_seconds );

    // get total text length
    int text_length = pct_text.length() + ela_text.length() + rem_text.length();

    // get desired bar length
    int bar_length = b_width * b_bar_width_fraction;
    detail::bound( b_minimum_bar_width, bar_length, bar_length );

    // adjust what we show if it would overflow columns available
    int total_length = text_length + bar_length + 2;
    int over_length = total_length - b_width;
    if( over_length > 0 )
    {
        // remove bar if no room
        bar_length -= over_length;
        if( bar_length < b_minimum_bar_width )
        {
            bar_length = 0;
            total_length -= bar_length + 2;
        }

        // remove rem_text if still no room
        over_length = total_length - b_width;
        if( over_length > 0 )
        {
            total_length -= rem_text.length();
            rem_text = "";
            // remove ela_text if still no room
            over_length = total_length - b_width;
            if( over_length > 0 )
            {
                total_length -= ela_text.length();
                ela_text = "";
            }
        }
    }

    // construct bar components
    int fill_length = bar_length * progress_fraction;
    std::string bf_text = bar_text( fill_length, b_fill_char );

    int empty_length = bar_length - fill_length;
    std::string be_text = bar_text( empty_length, b_empty_char );

    // recalculate total
    total_length = bar_length + text_length + 2;

    // padding at the end
    int pad_length = b_width - total_length;
    std::string pad_text;
    if( pad_length > 0 ) pad_text = std::string( pad_length, ' ' );

    auto& os = b_info.os();

    // pct_text
    // columns: 0 - 5
    // 100%
    //_____
    if( b_colorize ) os << color( b_fill_color, NO_CHANGE );
    os << pct_text;
    if( b_colorize ) os << color::clear;

    if( bar_length > 0 )
    {
        // bf_text
        // columns: 6 - LBF
        //[||||||||
        //_________
        if( b_colorize ) os << color( WHITE, NO_CHANGE );
        os << b_left_bookend;
        if( b_colorize ) os << color( NO_CHANGE, b_fill_color );
        os << bf_text;
        if( b_colorize ) os << color::clear;

        // be_text
        // columns: LBF+1 - LBE
        //        ]
        //_________
        if( b_colorize ) os << color( NO_CHANGE, b_empty_color );
        os << be_text;
        if( b_colorize )
        {
            os << color::clear;
            os << color( WHITE, NO_CHANGE );
        }
        os << b_right_bookend;
    }

    // ela_text
    // columns: LBE+1 - LELA
    // 55.2m elapsed
    //______________
    if( b_colorize ) os << color( b_fill_color, NO_CHANGE );
    os << ela_text;
    if( b_colorize ) os << color::clear;

    // rem_text
    // columns: LELA+1 - LREM
    // (11.3m left)
    //_____________
    if( b_colorize ) os << color( b_empty_color, NO_CHANGE );
    os << rem_text;
    if( b_colorize ) os << color::clear;
    os << pad_text;
}

progress_bar& progress_bar::set_left_bookend( char c )
{
    b_left_bookend = c;
    return *this;
}
char progress_bar::left_bookend() const { return b_left_bookend; }
progress_bar& progress_bar::set_right_bookend( char c )
{
    b_right_bookend = c;
    return *this;
}
char progress_bar::right_bookend() const { return b_right_bookend; }
progress_bar& progress_bar::set_empty_char( char c )
{
    b_empty_char = c;
    return *this;
}
char progress_bar::empty_char() const { return b_empty_char; }
progress_bar& progress_bar::set_fill_char( char c )
{
    b_fill_char = c;
    return *this;
}
char progress_bar::fill_char() const { return b_fill_char; }
progress_bar& progress_bar::set_empty_color( int c )
{
    b_empty_color = c;
    return *this;
}
int progress_bar::empty_color() const { return b_empty_color; }
progress_bar& progress_bar::set_fill_color( int c )
{
    b_fill_color = c;
    return *this;
}
int progress_bar::fill_color() const { return b_fill_color; }
progress_bar& progress_bar::set_minimum_bar_width( int w )
{
    b_minimum_bar_width = w;
    return *this;
}
int progress_bar::minimum_bar_width() const { return b_minimum_bar_width; }
progress_bar& progress_bar::set_width( int w )
{
    b_width = w;
    return *this;
}
int progress_bar::width() const { return b_width; }
progress_bar& progress_bar::set_bar_width_fraction( double f )
{
    detail::bound( 0.0, f, 1.0 );
    b_bar_width_fraction = f;
    return *this;
}

const info& progress_bar::info() const { return b_info; }
double progress_bar::bar_width_fraction() const { return b_bar_width_fraction; }
}  // term

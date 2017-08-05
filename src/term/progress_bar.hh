#ifndef Loggerhead_term_progress_bar_HH
#define Loggerhead_term_progress_bar_HH

#include "color.hh"
#include "info.hh"

namespace term
{
class progress_bar
{
  public:
    progress_bar( std::ostream& os );
    std::ostream& os();

    // change output stream
    void reset( std::ostream& os );

    // output progress bar
    virtual void output( double progress_fraction,
                         double elapsed_seconds = -1.0,
                         double remaining_seconds = -1.0 );

    //-----------------
    // change appearance
    //-----------------

    progress_bar& set_left_bookend( char );
    char left_bookend() const;

    progress_bar& set_right_bookend( char );
    char right_bookend() const;

    progress_bar& set_empty_char( char );
    char empty_char() const;

    progress_bar& set_fill_char( char );
    char fill_char() const;

    //------------
    // change color
    //------------

    progress_bar& set_colorize( color::Enum e );
    bool colorize() const;

    progress_bar& set_empty_color( int );
    int empty_color() const;

    progress_bar& set_fill_color( int );
    int fill_color() const;

    //------------------
    // change size of bar
    //------------------

    progress_bar& set_minimum_bar_width( int );
    int minimum_bar_width() const;

    progress_bar& set_width( int );
    int width() const;

    progress_bar& set_bar_width_fraction( double );
    double bar_width_fraction() const;

    // get terminal info
    const ::term::info& info() const;

  protected:
    // formatters used internally
    static std::string percentage_text( double progress_fraction );
    static std::string elapsed_text( double elapsed_seconds );
    static std::string remaining_text( double remaining_seconds );
    static std::string time_text( double seconds );
    static std::string bar_text( int, char );

  private:
    std::ostream* b_os;
    ::term::info b_info;
    char b_empty_char;
    int b_empty_color;
    char b_fill_char;
    int b_fill_color;
    char b_left_bookend;
    char b_right_bookend;
    int b_minimum_bar_width;
    int b_width;
    double b_bar_width_fraction;
    bool b_colorize;

};  // end class progress_bar

}  // term

#endif

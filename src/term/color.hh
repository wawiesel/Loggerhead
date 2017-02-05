#ifndef Testing123_term_color_HH
#define Testing123_term_color_HH

#include <iostream>

namespace term
{
// color enums for setting foreground or background
static const int NO_CHANGE = -1;
static const int BLACK = 0;
static const int RED = 1;
static const int GREEN = 2;
static const int YELLOW = 3;
static const int BLUE = 4;
static const int MAGENTA = 5;
static const int CYAN = 6;
static const int WHITE = 7;
static const int LAST_COLOR = 8;

// class which changes foreground and background colors on the command line
class color
{
  public:
    enum Enum
    {
        AUTO = -1,
        OFF = 0,
        ON = 1
    };
    static Enum DEFAULT_BEHAVIOR;

  public:
    // initialize color
    color( int fg, int bg = NO_CHANGE );
    // reverse foreground and background
    color reverse() const;
    // get foreground color integer
    int fg();
    // get background color integer
    int bg();
    // end line and clear background coloring
    static std::ostream& endl( std::ostream& os );
    // clear coloring
    static std::ostream& clear( std::ostream& os );
    // check
    static bool colorable( std::ostream&, Enum e = DEFAULT_BEHAVIOR );

  private:
    // makes the magic happen
    friend std::ostream& operator<<( std::ostream&, const color& );
    const int b_fg;
    const int b_bg;
};
}  // end namespace term
#endif  // Testing123_term_color_HH

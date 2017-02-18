#ifndef Loggerhead_term_info_HH
#define Loggerhead_term_info_HH

#include <ostream>

namespace term
{
class info
{
  public:
    info( std::ostream& os );
    bool is_tty() const;
    int rows() const;
    int columns() const;
    std::ostream& os();

  private:
    std::ostream* b_os;
    bool b_is_tty;
    int b_columns;
    int b_rows;
};

}  // term

#endif  // Loggerhead_term_info_HH

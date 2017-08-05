#ifndef Loggerhead_term_info_HH
#define Loggerhead_term_info_HH

#include <ostream>
#include <functional> /*reference_wrapper*/

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
    std::reference_wrapper<std::ostream> b_os;
    bool b_is_tty;
    int b_columns;
    int b_rows;
};

}  // term

#endif  // Loggerhead_term_info_HH

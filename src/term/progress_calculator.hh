#ifndef Loggerhead_term_progress_calculator_HH
#define Loggerhead_term_progress_calculator_HH

#include <chrono>

namespace term
{
class progress_calculator
{
  public:
    progress_calculator( int total_tasks );

    void reset( int total_tasks );

    void update( int dtasks );

    void finish();
    bool is_finished() const;

    double elapsed_seconds() const;
    double remaining_seconds() const;

    void set_total_tasks( int total_tasks );
    int remaining_tasks() const;
    int total_tasks() const;
    int completed_tasks() const;

    double fraction_completed() const;

  private:
    int b_total_tasks;
    int b_completed_tasks;
    bool b_is_finished;
    std::chrono::time_point<std::chrono::system_clock> b_start;
    double b_final_elapsed;

};  // end class calculator

}  // term

#endif

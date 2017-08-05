#ifndef Loggerhead_spdlog_sinks_progress_sink_HH
#define Loggerhead_spdlog_sinks_progress_sink_HH

#include "spdlog/sinks/stdout_sinks.h"
#include "term/progress_bar.hh"
#include "term/progress_calculator.hh"

namespace spdlog
{
namespace sinks
{
// inherits the capabilities of the progress calculator
// includes the geometry bar
template <class Mutex>
class progress_sink : public stderr_sink<Mutex>,
                      public term::progress_calculator
{
    using MyType = progress_sink<Mutex>;

  public:
    term::progress_bar bar;
    static std::shared_ptr<MyType> instance();
    static std::shared_ptr<MyType> instance(int total_tasks);
    void _sink_it( const details::log_msg& msg ) override;
    void flush() override;
    bool is_tty() const;

    void autoupdate();  // completes one task every time or pass through
    void set_allow_autoupdate( bool allow );
    bool allow_autoupdate() const;

    progress_sink();

  protected:
    bool b_allow_autoupdate{false};

};  // end class progress_sink

typedef progress_sink<std::mutex> progress_sink_mt;
typedef progress_sink<details::null_mutex> progress_sink_st;

}  // sinks
}  // spdlog

#include "progress_sink.i.hh"

#endif

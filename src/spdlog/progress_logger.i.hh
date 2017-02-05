#include "spdlog/details/spdlog_impl.h"
#include "spdlog/sinks/progress_sink.hh"

namespace spdlog
{
namespace
{
// common operations for functions below
template <typename sink>
std::shared_ptr<spdlog::logger> progress_logger( const std::string& logger_name,
                                                 sink s,
                                                 int total_tasks,
                                                 bool color )
{
    bool is = s->is_tty();
    if( color ) color = is;
    auto x = create_console_logger( logger_name, s, color );
    return x;
}

}  // anon

std::shared_ptr<spdlog::logger> progress_logger_mt(
    const std::string& logger_name, int total_tasks, bool color )
{
    auto s = sinks::progress_sink_mt::instance( total_tasks );
    return progress_logger( logger_name, s, total_tasks, color );
}

std::shared_ptr<spdlog::logger> progress_logger_st(
    const std::string& logger_name, int total_tasks, bool color )
{
    auto s = sinks::progress_sink_st::instance( total_tasks );
    return progress_logger( logger_name, s, total_tasks, color );
}

}  //spdlog

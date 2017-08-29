#include "spdlog/spdlog.h"
#include "spdlog/sinks/progress_sink.hh"

#ifdef _WIN32
#include "spdlog/sinks/wincolor_sink.h"
#else
#include "spdlog/sinks/ansicolor_sink.h"
#endif


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
    if( color )
    {
		std::vector<spdlog::sink_ptr> sinks;
#ifdef _WIN32
		auto color_sink = std::make_shared<spdlog::sinks::wincolor_sink>(s);
#else
		auto color_sink = std::make_shared<spdlog::sinks::ansicolor_sink>(s);
#endif
		sinks.push_back(color_sink);
		return create(logger_name, begin(sinks), end(sinks));
    }
    return spdlog::create( logger_name, s );
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

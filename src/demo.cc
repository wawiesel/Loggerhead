#include "spdlog/spdlog.h"
#include <spdlog/fmt/ostr.h> // must be included

#include <iostream>
#include <memory>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

namespace spd = spdlog;
/*
namespace spdlog {
namespace sinks {

class progress_calculator {
  public:
    progress_calculator( int total_tasks );
    void update( int dtasks );
    void finish( );
    void reset( int total_tasks );
    void set_total_tasks( int total_tasks );

    void autoupdate( ); //completes one task
    void set_allow_autoupdate( bool allow );
    bool allow_autoupdate() const;

    double elapsed_seconds() const;
    double remaining_seconds() const;
    int remaining_tasks() const;
    int total_tasks() const;
    int completed_tasks() const;
    double fraction_completed() const;

    bool is_finished() const;

  private:
    int b_total_tasks;
    int b_completed_tasks;
    bool b_is_finished;
    std::chrono::time_point<std::chrono::system_clock> b_start;
    double b_final_elapsed;
};

//geometry aspect
class progress_geometry {
  public:
    void reset( FileHandle* );
    void set_bookends(char left, char right);
    void set_empty(char e, color c);
    void set_full(char f, color c);
    void set_bar_size( int min_cols, float min_pct, int max_cols, float max_pct );
};

template <class Mutex>
class progress_sink : public stderr_sink<Mutex>, public progress_calculator
{
    using MyType = progress_sink<Mutex>;
public:
    progress_geometry bar;
    progress_sink() : progress_calculator(100) {
        bar.reset(stderr)
    }
    static std::shared_ptr<MyType> instance()
    {
        static std::shared_ptr<MyType> instance = std::make_shared<MyType>();
        return instance;
    }

    void _sink_it(const details::log_msg& msg) override
    {
        fwrite(msg.formatted.data(), sizeof(char), msg.formatted.size(), stderr);
        flush();
    }

    void flush() override
    {
        fflush(stderr);
    }

    bool isa_tty() const
    {
#ifdef _WIN32
    return ::_isatty( _fileno( stderr ) );
#else
    return ::isatty( fileno( stderr ) );
#endif
    }
};
typedef progress_sink<std::mutex> progress_sink_mt;
typedef progress_sink<details::null_mutex> progress_sink_st;
}//sinks

template <typename sink>
std::shared_ptr<spdlog::logger> progress_logger(const std::string& logger_name, sink s, bool color)
{
    bool is = s->isa_tty();
    if( color )color = is;
    auto x = create_console_logger(logger_name, s, color);
    if( is )x->set_pattern("\r%+");
    return x;
}

std::shared_ptr<spdlog::logger> progress_logger_mt(const std::string& logger_name, bool color=true)
{
    auto s = sinks::progress_sink_mt::instance();
    return progress_logger(logger_name,s,color);
}

std::shared_ptr<spdlog::logger> progress_logger_st(const std::string& logger_name, bool color=true)
{
    auto s = sinks::progress_sink_st::instance();
    return progress_logger(logger_name,s,color);
}
}//spdlog
*/

// user defined types logging by implementing operator<<
struct my_type
{
    int i;
    template<typename OStream>
    friend OStream& operator<<(OStream& os, const my_type &c)
    {
        os << std::string(c.i,'x') << std::flush;
        system("sleep 0.05s");
        return os;
    }
};


int main(int, char*[])
{
    // Console logger with color
    auto stdout = spd::stdout_logger_mt( "stdout", /*color=*/true );
    stdout->info( "Welcome to spdlog!" );
    stdout->error( "Some error message with arg{}..", 1 );
    stdout->set_level( spdlog::level::trace );
    stdout->warn( "Easy padding in numbers like {:08d}", 12 );
    stdout->error(
        "Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}",
        42 );
    stdout->info( "Support for floats {:03.2f}", 1.23456 );
    stdout->info( "Positional args are {1} {0}..", "too", "supported" );
    stdout->info( "{:<30}", "left aligned" );
    stdout->trace( "trace me {:03d}", 7 );

    auto progress = spd::stderr_logger_mt( "stderr", /*color=*/true );
    //auto progress = spd::progress_logger_mt( "progress" );
    for(int i=0; i<100; ++i)
    {
        progress->warn("{:<130}",fmt::format("{}",i));
        std::cerr << my_type{i};
    }
    std::cerr << "\r" << my_type{100} <<std::endl;
    return 0;
}

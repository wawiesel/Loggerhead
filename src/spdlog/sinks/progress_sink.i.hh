#include <cstdio>
#include <sys/ioctl.h>
#include <unistd.h>
#include "term/info.hh"

namespace spdlog
{
namespace sinks
{
template <class Mutex>
progress_sink<Mutex>::progress_sink()
    : bar(std::cerr), progress_calculator(1)
{
}

template <class Mutex>
std::shared_ptr<progress_sink<Mutex>> progress_sink<Mutex>::instance()
{
    static std::shared_ptr<progress_sink<Mutex>> instance =
        std::make_shared<progress_sink<Mutex>>();
    return instance;
}

template <class Mutex>
void progress_sink<Mutex>::autoupdate()
{
    if( b_allow_autoupdate )
    {
        update( -1 );
    }
}

template <class Mutex>
void progress_sink<Mutex>::set_allow_autoupdate( bool allow )
{
    b_allow_autoupdate = allow;
}

template <class Mutex>
bool progress_sink<Mutex>::allow_autoupdate() const
{
    return b_allow_autoupdate;
}

template <class Mutex>
void progress_sink<Mutex>::update( int dtasks )
{
    term::progress_calculator::update( dtasks );

    if( bar.width()>0 )std::cerr << "\r" << std::string(bar.width(),' ');
    std::cerr << "\r";

    bar.output( this->fraction_completed(), this->elapsed_seconds(), this->remaining_seconds() );
    flush();
}
template <class Mutex>
std::shared_ptr<progress_sink<Mutex>> progress_sink<Mutex>::instance(int total_tasks)
{
    auto sink = instance();
    sink->reset(total_tasks);
    sink->set_allow_autoupdate(true);
    return sink;
}

template <class Mutex>
void progress_sink<Mutex>::_sink_it( const details::log_msg& msg )
{
    this->autoupdate();

    if( bar.width()>0 )std::cerr << "\r" << std::string(bar.width(),' ');
    std::cerr << "\r";

    fwrite(
        msg.formatted.data(), sizeof( char ), msg.formatted.size(), stderr );
    bar.output( this->fraction_completed(), this->elapsed_seconds(), this->remaining_seconds() );
    flush();
}

template <class Mutex>
void progress_sink<Mutex>::flush()
{
    fflush( stderr );
}
template <class Mutex>
bool progress_sink<Mutex>::is_tty() const
{
    return term::info( std::cerr ).is_tty();
}

}  // sinks
}  // spdlog

#include "progress_sink.hh"

namespace spdlog
{
namespace sinks
{

template <>
class progress_sink<std::mutex>;
template <>
class progress_sink<details::null_mutex>;

}  // sinks
}  // spdlog

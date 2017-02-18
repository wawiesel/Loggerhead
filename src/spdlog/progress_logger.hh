#ifndef Loggerhead_spdlog_progress_logger_HH
#define Loggerhead_spdlog_progress_logger_HH

#include <memory>
#include <string>
#include "spdlog/logger.h"

namespace spdlog
{

std::shared_ptr<spdlog::logger> progress_logger_mt(
    const std::string& logger_name, int total_tasks, bool color = true );

std::shared_ptr<spdlog::logger> progress_logger_st(
    const std::string& logger_name, int total_tasks, bool color = true );

}  // spdlog

#include "progress_logger.i.hh"

#endif

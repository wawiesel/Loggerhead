Loggerhead
==========

Logging and terminal stuff with BOTG_ package management

.. image:: https://c1.staticflickr.com/1/606/33135230306_71e4f916c5_b.jpg

This package provides the SPDLOG_ package for logging with a few bell and
whistle classes.

- terminal stuff

  - information (screen width, is a TTY) with `term::terminal_info`
  - coloring  with `term::color`
  - ASCII (with color!) progress bar with `term::progress_bar`

- SPDLOG_ logging with a progress bar

  - single thread `spdlog::progress_logger_mt` or
  - multi thread `spdlog::progress_logger_st`

.. code_block: c++

    #include "spdlog/progress_logger.hh"

    int main()
    {
        //create a multi-threaded progress logger
        size_t n = 6;
        auto progress = spdlog::progress_logger_mt("demo",n);

        for(auto i=1; i<=n; ++i)
        {
            //simulate 100 ms
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

            //output [info] information about the task
            progress->info( "Completed task {}",i );
        }

        //go to next line
        std::cerr << "\n";

    }

.. _CMake: https://cmake.org/
.. _SPDLOG: https://github.com/gabime/spdlog
.. _BOTG: http://github.com/wawiesel/BootsOnTheGround
.. _Loggerhead: http://github.com/wawiesel/Loggerhead




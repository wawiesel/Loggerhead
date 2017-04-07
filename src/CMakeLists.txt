botgPackage( Loggerhead )

botgUseCxxStandard( 11 )

botgLibrary( Loggerhead
    SOURCES
        spdlog/sinks/progress_sink.cc

        term/color.cc
        term/info.cc
        term/progress_bar.cc
        term/progress_calculator.cc
    HEADERS
        spdlog/progress_logger.hh
        spdlog/progress_logger.i.hh

        spdlog/sinks/progress_sink.hh
        spdlog/sinks/progress_sink.i.hh
 
        term/color.hh
        term/info.hh
        term/progress_bar.hh
        term/progress_calculator.hh
)

botgTestDir(
    spdlog/test
    term/test
)

botgEnd()

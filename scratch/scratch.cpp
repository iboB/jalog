#include <jalog/DefaultLogger.hpp>
#include <jalog/Logger.hpp>
#include <jalog/Log.hpp>
#include <jalog/LogPrintf.hpp>

#include <jalog/sinklib/ColorSink.hpp>
#include <jalog/AsyncLogging.hpp>
#include <jalog/AsyncLoggingThread.hpp>

#include <jalog/Printf.hpp>

int main()
{
    auto async = std::make_shared<jalog::AsyncLogging>();

    async->add<jalog::sinklib::ColorSink>();

    jalog::DefaultLogger().setup()
        .add(async);

    jalog::AsyncLoggingThread lt(*async);

    jalog::Scope Algos("Algorithms");

    int* p = new int[12];

    JALOG(Info, "Hello ", 3, " times");
    JALOG(Info, "Something or other");
    JALOG(Info, "P is ", p);
    JALOG_SCOPE(Algos, Info, "Initializing algos");
    JALOG(Debug, "iteration ", 0);
    JALOG(Debug, "iteration ", jalog::base<2>(53));
    JALOG_SCOPE(Algos, Warning, "Algos initialized with missing stuff");
    JALOG_PRINTF(Info, "Done %d", 43);
    JALOG(Warning, "Result was calculated but not really good ", 3.33);
    JALOG(Info, "Going further");
    JALOG(Error, "Something bad happened");
    JALOG(Critical, "Something REALLY bad happened");

    return 0;
}

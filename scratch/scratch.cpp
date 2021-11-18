#include <jalog/DefaultLogger.hpp>
#include <jalog/Logger.hpp>
#include <jalog/Log.hpp>

#include <jalog/sinks/SimpleStreamSink.hpp>

int main()
{
    jalog::DefaultLogger().setup()
        .addSink<jalog::sinks::SimpleConsoleSink>();

    JALOG(Info, "Hello ", 3, " times");

    return 0;
}

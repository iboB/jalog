#include <jalog/DefaultLogger.hpp>
#include <jalog/Logger.hpp>
#include <jalog/Log.hpp>

int main()
{
    jalog::DefaultLogger().setup();

    JALOG(Info, "Hello ", 3, " times");

    return 0;
}

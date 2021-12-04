#include <jalog/DefaultLogger.hpp>
#include <jalog/Instance.hpp>
#include <jalog/Log.hpp>
#include <jalog/LogPrintf.hpp>

#include <jalog/sinklib/ColorSink.hpp>

#include <jalog/Printf.hpp>

struct vec { int x, y; };

std::ostream& operator<<(std::ostream& o, const vec& v)
{
    return o << v.x << ' ' << v.y;
}

jalog::BasicStream& operator,(jalog::BasicStream& s, const vec& v)
{
    return s, '(', v.x, ';', v.y, ')';
}

int main()
{
    jalog::Instance jl;
    jl.setup().async().add<jalog::sinklib::ColorSink>();

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
    JALOG(Info, "Doing stuff: ", true);
    JALOG(Error, "Something bad happened");
    JALOG(Critical, "Something REALLY bad happened");

    vec v = {1, 2};
    JALOG(Info, "vec: ", v);

    return 0;
}

#include <jalog/Instance.hpp>
#include <jalog/sinks/ColorSink.hpp>
#include <jalog/Log.hpp>
#include <jalog/LogPrintf.hpp>

#include <string>
#include <string_view>

struct Person
{
    std::string name;
    int age;
};

std::ostream& operator<<(std::ostream& o, const Person& p)
{
    return o << p.name << '(' << p.age << ')';
}

int main()
{
    jalog::Instance jl;
    jl.setup().add<jalog::sinks::ColorSink>();

    JALOG(Debug, "Log integers: ", 34, ", or in a custom base: ", jalog::base<16>(255));
    JALOG(Info, "Log floating point numbers with no precision loss: ", 12.4356631);

    std::string str = "my string";
    std::string_view sv = std::string_view(str).substr(0, 6);
    JALOG(Warning, "Log strings: '", str, "' and string views '", sv, "'");

    Person alice = {"Alice", 34};
    JALOG(Error, "Log types with custom ostream output: ", alice);

    JALOG_PRINTF(Critical, "Log printf style: %d, %.3f, %s", 43, 3.14159, str.c_str());

    return 0;
}

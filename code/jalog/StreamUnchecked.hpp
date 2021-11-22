// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"

#include "_qwrite.hpp"

#include "Scope.hpp"

#include <ostream>
#include <type_traits>

namespace jalog
{

class StreamUnchecked
{
public:
    StreamUnchecked(Scope& scope, Level lvl)
        : m_scope(scope)
        , m_level(lvl)
        , m_stream(&m_streambuf)
    {}

    ~StreamUnchecked()
    {
        // length before zero-termination
        auto textLength = m_streambuf.poff();
        // we don't *have* to zero-terminate the string, but let's be friendly to the outside world
        m_streambuf.sputc(0);

        m_scope.addEntryUnchecked(m_level, std::string_view(m_streambuf.peek_container().data(), textLength));
    }

    #define I_SU_O_INTEGER(t) \
    StreamUnchecked& operator,(t i) { \
        qwrite::write_integer(m_streambuf, qwrite::wrapped_integer<t, 10>{i}); \
        return *this; }
    I_SU_O_INTEGER(signed char)
    I_SU_O_INTEGER(unsigned char)
    I_SU_O_INTEGER(signed short)
    I_SU_O_INTEGER(unsigned short)
    I_SU_O_INTEGER(signed int)
    I_SU_O_INTEGER(unsigned int)
    I_SU_O_INTEGER(signed long)
    I_SU_O_INTEGER(unsigned long)
    I_SU_O_INTEGER(signed long long)
    I_SU_O_INTEGER(unsigned long long)
    #undef I_SU_O_INTEGER

    StreamUnchecked& operator,(char c)
    {
        m_streambuf.sputc(c);
        return *this;
    }

    template <typename T>
    StreamUnchecked& operator,(const T& t)
    {
        m_stream << t;
        return *this;
    }

private:
    Scope& m_scope;
    Level m_level;

    qwrite::streambuf m_streambuf;
    std::ostream m_stream;
};

}

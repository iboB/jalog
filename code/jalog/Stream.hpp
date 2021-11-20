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

namespace jalog
{

class Stream
{
public:
    Stream(Scope& scope, Level lvl)
        : m_scope(scope)
        , m_level(lvl)
        , m_stream(&m_streambuf)
    {}

    ~Stream()
    {
        // length before zero-termination
        auto textLength = m_streambuf.poff();
        // we don't *have* to zero-terminate the string, but let's be friendly to the outside world
        m_streambuf.sputc(0);

        m_scope.addEntry(m_level, std::string_view(m_streambuf.peek_container().data(), textLength));
    }

    template <typename T>
    Stream& operator,(const T& t)
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

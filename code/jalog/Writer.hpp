// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"

#include "Scope.hpp"

#include <itlib/small_vector.hpp>
#include <itlib/mem_streambuf.hpp>
#include <ostream>

namespace jalog
{

class Writer
{
public:
    Writer(Scope& scope, Level lvl)
        : m_scope(scope)
        , m_level(lvl)
        , m_stream(&m_streambuf)
    {}

    ~Writer()
    {
        // length before zero-termination
        auto textLength = m_streambuf.poff();
        // we don't *have* to zero-terminate the string, but let's be friendly to the outside world
        m_streambuf.sputc(0);

        m_scope.addEntry(m_level, std::string_view(m_streambuf.peek_container().data(), textLength));
    }

    template <typename T>
    Writer& operator,(const T& t)
    {
        m_stream << t;
        return *this;
    }

private:
    Scope& m_scope;
    Level m_level;

    itlib::mem_ostreambuf<itlib::small_vector<char, 1024>> m_streambuf;
    std::ostream m_stream;
};

}

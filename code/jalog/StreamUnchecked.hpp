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

template <unsigned Base, typename I>
auto base(I i) -> qwrite::wrapped_integer<I, Base> { return {i}; }

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

    using Self = StreamUnchecked;

    Self& operator,(char c) { m_streambuf.sputc(c); return *this; }
    Self& operator,(std::string_view s) { m_streambuf.sputn(s.data(), s.size()); return *this; }
    Self& operator,(const char* s) { operator,(std::string_view(s)); return *this; }
    Self& operator,(const std::string& s) { operator,(std::string_view(s)); return *this; }

    template <typename T>
    Self& operator,(T* p)
    {
        qwrite::write_integer(m_streambuf,
            qwrite::wrapped_integer<uintptr_t, 16>{reinterpret_cast<uintptr_t>(p)});
        return *this;
    }

    // baseful and padded integers
    template <typename I, unsigned Base>
    Self& operator,(qwrite::wrapped_integer<I, Base> wi) {
        qwrite::write_integer(m_streambuf, wi);
        return *this;
    }

    template <typename T>
    Self& operator,(const T& t)
    {
        if constexpr (std::is_integral_v<T>)
            qwrite::write_integer(m_streambuf, qwrite::wrapped_integer<T, 10>{t});
        else if constexpr (std::is_floating_point_v<T>)
            qwrite::write_float(m_streambuf, t);
        else
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

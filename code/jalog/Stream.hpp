// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "BasicStream.hpp"

namespace jalog
{

class Stream : public BasicStream
{
public:
    Stream(Scope& scope, Level lvl = Level::Info)
        : BasicStream(scope, lvl)
        , m_enabled(scope.enabled(lvl))
    {}

    ~Stream()
    {
        flushIfNotEmpty();
    }

    bool enabled() const { return m_enabled; }

    Stream& operator<<(Level lvl)
    {
        flushIfNotEmpty();
        m_level = lvl;
        m_enabled = m_scope.enabled(lvl);
        return *this;
    }

    template <typename T>
    Stream& operator<<(const T& t)
    {
        if (!m_enabled) return *this;
        BasicStream::operator,(t);
        return *this;
    }

private:
    void flushIfNotEmpty()
    {
        if (m_streambuf.poff() != 0)
        {
            flush();
        }
    }

    bool m_enabled;
};

}

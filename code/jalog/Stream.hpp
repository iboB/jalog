// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
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
        static_cast<BasicStream&>(*this), t;
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

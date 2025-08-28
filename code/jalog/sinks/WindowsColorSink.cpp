// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "WindowsColorSink.hpp"

#include "SimpleStdioSink.hpp"
#include "../Entry.hpp"

#include <itlib/time_t.hpp>

#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincon.h>


namespace jalog::sinks
{

WindowsColorSink::WindowsColorSink()
{
    DWORD unused;
    m_out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleMode(m_out, &unused)) m_out = nullptr; // out is not a console
    m_err = GetStdHandle(STD_ERROR_HANDLE);
    if (!GetConsoleMode(m_err, &unused)) m_err = nullptr; // err is not a console
}

namespace
{
void printWithColor(HANDLE out, uint16_t color, std::string_view text)
{
    uint16_t originalAttribs;

    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(out, &info))
    {
        // fallback to white
        originalAttribs = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
    else
    {
        originalAttribs = info.wAttributes;
    }

    WORD newAttributes = color | (originalAttribs & 0xfff00);
    SetConsoleTextAttribute(out, newAttributes);

    WriteConsoleA(out, text.data(), DWORD(text.size()), nullptr, nullptr);

    // return to normal
    SetConsoleTextAttribute(out, originalAttribs);
}

}

void WindowsColorSink::record(const Entry& entry)
{
    auto out = entry.level < Level::Error ? m_out : m_err;

    if (!out)
    {
        // output is not a console
        // do a redirect
        sinks::SimpleStdioSink(stdout, stderr).record(entry);
        return;
    }

    // time
    {
        using namespace std::chrono;
        itlib::time_t tt(system_clock::to_time_t(entry.timestamp));
        auto tm = tt.localtime();

        char buf[26];
        strftime(buf, sizeof(buf), "%m/%d %X", &tm);

        int ms = int(duration_cast<milliseconds>(entry.timestamp.time_since_epoch()).count() % 1000);
        char buf2[32];
        int len = snprintf(buf2, sizeof(buf2), "%s.%03d ", buf, ms);

        printWithColor(out, FOREGROUND_BLUE, std::string_view(buf2, size_t(len)));
    }

    // scope
    {
        auto lbl = entry.scope.label();
        if (!lbl.empty())
        {
            printWithColor(out, FOREGROUND_GREEN | FOREGROUND_BLUE, lbl);
            WriteConsoleA(out, " ", 1, nullptr, nullptr);
        }
    }

    // level
    {
        static const uint16_t colors[] = {
            FOREGROUND_RED | FOREGROUND_BLUE,
            FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            FOREGROUND_RED,
            FOREGROUND_RED | FOREGROUND_INTENSITY
        };
        static_assert(std::size(colors) == static_cast<uint32_t>(Level::Off));
        uint16_t color = (entry.level < Level::Off) ? colors[static_cast<uint32_t>(entry.level)] : (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        static const std::string_view lstr[] = {
            "[debug] ",
            "[Info ] ",
            "[WARN ] ",
            "[ERROR] ",
            "[CRIT!] ",
        };
        static_assert(std::size(lstr) == static_cast<uint32_t>(Level::Off));
        std::string_view str = (entry.level < Level::Off) ? lstr[static_cast<uint32_t>(entry.level)] : std::string_view("[  ?  ]");

        printWithColor(out, color, str);
    }

    WriteConsoleA(out, entry.text.data(), DWORD(entry.text.length()), nullptr, nullptr);
    WriteConsoleA(out, "\n", 1, nullptr, nullptr);
}

}

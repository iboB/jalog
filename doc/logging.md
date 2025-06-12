# Logging

In Jalog the use of macros for logging is recommended (though not required). The primary benefit of macros is that they allow you to perform checks at the call site. Thus, for example, if logging is disabled the arguments of the logging call won't be evaluated at all.

It is a good idea for users to create their own macros, suitable for their project and needs.

## Jalog Style Logging

This is the "flagship" way of doing logging with Jalog: the macros `JALOG` and `JALOG_SCOPE` which were mentioned and used in the previous doc pages. `JALOG` is implementd with `JALOG_SCOPE` by providing the default scope as a first argument (see more on [scopes](creating-scopes.md) here).

```
JALOG_SCOPE(scope, unscoped_level, ...)
JALOG(unscoped_level, ...) => JALOG_SCOPE(::jalog::Default_Scope, unscoped_level, ...)
```

Here `unscoped_level` is one of Jalog's levels but without the need to specify `::jalog::Level` before that. This is done in the macro for easer writing and reading.

The rest of the arguments are concatenated to produce the log message in a way similar to JavaScript's `console.log` (note however that `console.log` in JS adds spaces between arguments, while Jalog does not). Thus all of these examples will produce the same log message:

```c++
JALOG(Info, "Hello, World!")
JALOG(Info, "Hello, ", "World!")
JALOG(Info, "Hel", "lo,", " Wor", "ld!")
```

As for the types of those arguments, Jalog offers build-in support for:

* C++ built-in numeric types (`int`, `long`, `float`...)
* `bool` - printed as `true` or `false`. If you want bools printed as integers, you can cast them at the call site.
* Pointers - printed as hexadecimal values
* `char` - printed as a character
* Strings - `const char*`, `std::string`, `std::string_view`
* Integers in a specified base via `jalog::base<>`
* Any other type which has a defined `operator <<(std::ostream&, T)`

### Logging Custom Types

As mentioned above Jalog will log types which have defined output for `std::ostream`. If you want to log your own custom type, defining `operator <<` for it will work. However this is not the preferred method of defining custom output for Jalog. It's only there for backwards compatibility.

The problems with `std::ostream` are many and have been discussed in various places with various detail. We will synthesize them to two:

* Performance - `std::ostream` is slow and relies on many indirections and inefficiencies to do basic things
* Correctness - `std::ostream` is bad when it has to display floating point numbers. It will mangle them and lose precision sometimes terribly so. And it's hard to impossible to prevent it from doing so.

That's why the preferred way of adding Jalog output to a custom type is overloading `operator, (jalog::BasicStream&, T)` (that's operator comma). Inside use operator comma for all the parts of the custom type you want to log, much like you would use `operator <<` in a custom `std::ostream` output function.

Here's an example:

```c++
struct Person
{
    std::string name;
    int age;
};

jalog::BasicStream& operator,(jalog::BasicStream& s, const Person& p)
{
    return s, p.name, '(', p.age, ')';
}
```

In most cases this will be significantly faster than using `std::ostream` (and never slower). It will also display floating-point numbers correctly.

## Stream Style Logging

Jalog also supports stream-style logging with `operator<<`. Note that this is not done by inheriting from `std::ostream`. Jalog provides its own class `jalog::Stream` which has an overload for `operator<<`.

The benefit of `jalog::Stream` is that it can be used to compose a single log message from multiple lines of code. However this comes at the cost of being a bit slower to compile than the Jalog Style logging from above and, most notably, even if it is disabled at compile-time the arguments of its `operator<<` (the values you log) will still be evaluated when you run the program, potentially making it a bit slower.

To use Stream Style logging with macros (which makes it easier to disable at compile time), you must include `jalog/LogStream.hpp`. The the available macros are `JALOG_STREAM(level)` and `JALOG_STREAM_SCOPE(scope, level)` for custom scopes.

You can use them inline like this:

```c++
JALOG_STREAM(Info) << "Logging something: " << 343;
```

... but the more useful approach, which helps with multi-line logging, is to create a variable like so:

```c++
auto log = JALOG_STREAM(Info);
log << "Logging something: ";
log << 343 << jalog::endl;
log << jalog::Level::Error << "Now an error ";
log << "error continued" << jalog::endl;
```

Note two things from the examples above.

* `jalog::Stream` allows changing of the level, by simply "logging" the new desired level. After this the level in which new things are logged will be changed.
* `jalog::Stream` will flush the message to the scope (and subsequently to sinks) if one of three conditions is met:
    * Its destructor is called
    * You use `jalog::endl`
    * You change the level. In this case the contents of the log, if any, will be fluished for the old level

This allows users to create a single `jalog::Stream` instance which can be used for log period (say the entire lifetime of the application)

A complete, buildable example, of Stream Style logging can be found [here](../example/e-StreamStyleLogging.cpp).

### Custom types

You can log custom types with `jalog::Stream` the same you can with Jalog Style logging. Both `operator<<(std::ostream&, T)` and `operator, (jalog::BasicStream&, T)` are supported, and the `operator,` approach is preferred if possible.

### No macros

If you have no intention to disable stream logs in compile time, or if you have an alternative way of doing so (for example putting your Stream Style logging in preprocessor `#if`-s) there is no need to use the `JALOG_STREAM` macros and instead you can simply include `jalog/Stream.hpp` and use the `jalog::Stream` like an ordinary C++ class. You must provide a scope to its constructor (more on scopes [here](creating-scopes.md))

```c++
jalog::Stream log(jalog::Default_Scope);
log << jalog::Level::Info << "This works" << jalog::endl;
log << jalog::Level::Error << "So does this" << jalog::endl;
```

## `printf` Style Logging

Jalog supports `printf` style logging.

If you are a fan of `printf` you can use the macros from `jalog/LogPrintf.hpp` which are easy to disable at compile-time. Note however that `printf` doesn't support custom types and also floating point numbers won't appear in their full native precision (but must be formatted with `%f` or `%e`).

The main goal of the `printf`-like interface is to allow users to provide a function pointer to libraries (including plain C ones) which support external `printf`-like log functions.

With `jalog/Printf.hpp` Jalog provides the following functions:

* `void Printf(Scope& scope, Level lvl, const char* format, ...);`
* `void VPrintf(Scope& scope, Level lvl, const char* format, va_list args);`

These functions are templates with a default argument. The argument itself is a flags integer which supports the follwing flags:

* `PrintfFlags::SkipLogLevelCheck` - Skip checking whether the log level is enabled. This is useful if you have already checked it before calling the function. For example the `JALOG_PRINTF` macros perform the check externally and use the unchecked variants of the functions
* `PrintfFlags::TrimTrailingNewline` - Trim the trailing newline from the log message. This might be useful when you're replacing existing `printf`-like logging which requires newlines at the end of the log message. 
* `PrintfFlags::None` - The default: checks the log level from the scope and doesn't touch trailing newlines.

A complete, buildable example, of using `printf` logging to interface with an imaginary C library can be found [here](../example/e-PrintfStyleLogging.cpp).

## Direct logging. Custom logging styles

Logging styles are essentially different ways in which one can produce a string. If you have a custom way of producing strings, it can also be used with Jalog.

`jalog::Scope` provides two methods for this:

* `addEntry(level, string)` - log string if level is enabled
* `addEntryUnchecked(level, string)` - log string with level *disregarding* whether level is enabled. You can use this if you have performed the check otherwise.

Thus one can create a custom style of logging with Jalog with another string-producing library. A good candidate for this is C++20's `std::format`.

A complete, buildable example, of custom logging with `std::ostringstream` can be found [here](../example/e-CustomStyleLogging.cpp).

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

Jalog also supports stream-style logging with `operator<<`.

## `printf` Style Logging

Jalog supports `printf` style logging and with it can be used to provide a function pointer to libraries (including plain C ones) which support external printf-style log functions.

# Features and Glossary

Jalog is a logging library written with performance in mind. It doesn't provide a lot of features but is easy-to-use and is extendable.

Jalog supports synchronous and asynchronous logging and is generally thread safe (though it is possible to create sinks which are not thread safe).

Jalog promotes (though does not require) the use of macros for logging as they allow better flexibility in terms of what is compiled and what not. Another benefit of using macros in this case is that we can afford to wrap the underlying call in an if-check at the call site.

## Levels

Jalog supports a fixed set of log levels:

* Debug
* Info
* Warning
* Error
* Critical

Levels are used for log message filtering. Debug is the lowest level and Critical is the highest. Setting a "minimal level" means disabling logs with a level less than the provided.

In Jalog the Level is simply an enum (`jalog::Level`), with an additional member `Off` to show that all log levels need to be turned off.

## Scopes

Scopes are named categories for log messages. Each log message must be associated with one scope.

Scopes in Jalog are simply instances of the class `jalog::Scope`.

The library provides a default scope instance.

## Sinks

Sinks are the places where the log messages "go". `jalog::Sink` is a class with two virtual methods to accept a log entry and to flush. Sinks are the means to customize and extend the library.

The library itself provides [several general purpose sinks](sinks.md).

## Loggers

The Logger is the central instance where sinks and scopes are registered. Typically the built-in default logger is used for everything, but Jalog supports multiple loggers.

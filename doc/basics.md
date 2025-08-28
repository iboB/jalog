# Jalog Basics

This is a tutorial on how to setup Jalog with the built-in default logger and synchronous logging.

## Simply Start Logging

Once you build Jalog and link your app with it, you can start logging. The easiest way to do it is via the `JALOG` macro (for others and more on logging, see [here](logging.md)).

The macro takes a log level (unscoped) as a first argument and concatenates all subsequent arguments in a log message.

Examples of this include:

```c++
JALOG(Info, "Hello world");
JALOG(Debug, "Some integer ", 5);
JALOG(Error, "Some value: ", value, " and another: ", another);
```

Jalog supports values which have an associated overloaded `operator<<` for `std::ostream`. Adding them as arguments will compile and work as expected.

A complete, buildable example can be found here [here](../example/e-BasicShowcase.cpp).

## Controling Sinks

The default logger is initially configured with the system-dependent `DefaultSink`. You can reconfigure it with your own set of sinks as follows:

At the entry point of your program (likely the `main` function) you need to create a `jalog::Instance`. And call its `setup` method. 

`jalog::Instance::setup` returns a `jalog::Instance::SetupDSL` which itself has these basic setup methods:

* `overrideInitialLevel(std::optional<jalog::Level>)` - override the initial for new scopes (more on scope creation [here](creating-scopes.md)).
* `template <T, ...Args> add(Args&&...)` - add a sink by type. This call forwards its arguments the arguments to the sink's constructor.
* `add(std::shared_ptr<Sink>)` - add a sink which already has a instance in a shared pointer (more on sink creation [here](custom-sinks.md)).
* `async()` - initiate setup of the built-in asynchronous logging (more about it [here](async.md))

The setup DSL object will perform the necessary steps in its destructor. This is the recommended way of using it:

```c++
jalog::Instance jl;
jl.setup()
    .add<jalog::sinks::ColorSink>()
    .add(mySink)
    .overrideInitialLevel(jalog::Level::Info);
```

The more verbose, but equivalent, alternative would be:

```c++
jalog::Instance jl;
{
    auto setup = jl.setup();
    setup.add<jalog::sinks::ColorSink>();
    setup.add(mysink);
    setup.overrideInitialLevel(jalog::Level::Info);
}
```

Besides the setup DSL, the instance itself only provides methods to get the underlying logger and async logging sink (if any) and in a straight-forward use case would not be touched any more until it goes out of scope and logging ends.


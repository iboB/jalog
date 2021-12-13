# Jalog Basics

This is a tutorial on how to setup Jalog with the built-in default logger and synchronous logging.

## Set-up an Instance

At the entry point of your program (likely the `main` function) you need to create a `jalog::Instannce`. And call its `setup` method. The lifetime of this object will determine the period in which logging will happen.

Any attempts to do logging outside of the lifetime of this object (though perfectly safe) will likely lead to the logs not happenning. To reiterate: trying to do logging outside of the lifetime of a `jalog::Instance` is safe. There will be no crash and no C++ undefined behavior will be involved, however the logs in this case will in most cases be equivalent to `if (false)`.

`jalog::Instance::setup` returns a `jalog::Instance::SetupDSL` which itself has these basic setup methods:

* `defaultLevel(jalog::Level)` - set the default level for the logger. This is relevant for newly created scopes (more on scope creation [here](creating-scopes.md)).
* `template <T, ...Args> add(Args&&...)` - add a sink by type. This call forwards its arguments the arguments to the sink's constructor.
* `add(std::shared_ptr<Sink>)` - add a sink which already has a instance in a shared pointer (more on sink creation [here](custom-sinks.md)).
* `async()` - initiate setup of the built-in asynchronous logging (more about it [here](async.md))

The setup DSL object will perform the necessary steps in its destructor. This is the recommended way of using it:

```c++
jalog::Instance jl;
jl.setup()
    .add<jalog::sinks::ColorSink>()
    .add(mySink)
    .defaultLevel(jalog::Level::Info);
```

The more verbose, but equivalent, alternative would be:

```c++
jalog::Instance jl;
{
    auto setup = jl.setup();
    setup.add<jalog::sinks::ColorSink>();
    setup.add(mysink);
    setup.defaultLevel(jalog::Level::Info);
}
```

Besides the setup DSL, the instance itself only provides methods to get the underlying logger and async logging sink (if any) and in a straight-forward use case would not be touched any more until it goes out of scope and logging ends.

## Start Logging

After an instance has been set up, you can now start logging. The easiest way to do it is via the `JALOG` macro (for others and more on logging, see [here](logging.md)).

The macro takes a log level (unscoped) as a first argument and concatenates all subsequent arguments in a log message.

Examples of this include:

```c++
JALOG(Info, "Hello world");
JALOG(Debug, "Some integer ", 5);
JALOG(Error, "Some value: ", value, " and another: ", another);
```

Jalog supports values which have an associated overloaded `operator<<` for `std::ostream`. Addin them as arguemnts will compile and work as expected.

## Complete Example

A complete, buildable example, covering these basics can be found [here](../example/e-HelloWorld.cpp).

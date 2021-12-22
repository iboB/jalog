# Asynchronous Logging

Asynchronous logging means that the log messages are not sent to sinks immediately, but are pushed to a queue to be sent later (likely in a different thread). Asynchronous logging has the benefit of not burdening the current thread with I/O  (or at least only do significantly less of it) at the cost the log "trailing" at least a bit behind the actual state of the software.

Jalog offers built-in support for asynchronous logging.

## Through `jalog::Instance`

The easiest way to enable asynchronous logging in jalog is to set it up in the [instance](basics.md#set-up-an-instance) with `async()`. Instead of calling `add()` when setting up, first call `async()` and then `add()`. Any sinks added before the `async()` call will be synchronous (called immediately when logging - in the same call stack) and the ones after it will be asynchronous. Like so:

```c++
jalog::Instance jl;
jl.setup()
    .add(mySink1) // not-asynchronous!
    .async()
        .add<jalog::sinks::ColorSink>()
        .add(mySink2);
```

This will lead to a separate thread being launched and log messages will be enqueued in to be consumed by the two asynchronous sinks.

## Manual Setup

Setting up asynchronous logging can also be done manually. You will need to create a shared pointer of `jalog::AsyncLogging` and then add it to a `jalog::Instance`'s setup as if it is a sink (which, by the way, it actually is). You can add (or remove) sinks from your `AsyncLogging` object at any time (as opposed to `jalog::Instance` which gives you this opportunity only once on setup).

This will add the asynchronous logging object to the instance, but there will be no logging thread launched.

You can start one by creating a `jalog::AsyncLoggingThread` object. It will start a thread in its constructor and stop it in its destructor.

Combining all of the above, we can achieve a behavior identical to `jalog::Instance::async()` but more verbose like this:

```c++
auto async = std::make_shared<jalog::AsyncLogging>();
jalog::AsyncLoggingThread thread(*async);
async->add<jalog::sinks::ColorSink>();
async->add(mySink);

jalog::Instance jl;
jl.setup().add(async);
```

Note that you don't *have* to start the built-in `AsyncLoggingThread`. You can manually call `update` of `AsyncLogging`. For example in a game, you can use `AsyncLogging` and call `update` in the render thread, while you don't burden the gameplay thread with I/O.

## Custom Asynchronous Logging

Of course you don't have to use the built-in asynchronous logging capabilities of Jalog and still have asynchronous logging. As we mentioned above `jalog::AsyncLogging` is just a `jalog::Sink`. You can create your own object derived from `jalog::Sink` to do asynchronous logging which is best suited for your needs and environment.

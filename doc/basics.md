# Jalog Basics

This is a tutorial on how to setup Jalog with the built-in default logger and synchronous logging. 

## Set-up an Instance

At the entry point of your program (likely the `main` function) you need to create a `jalog::Instannce`. And call its `setup` method. The lifetime of this object will determine the period in which logging will happen. 

Any attempts to do logging outside of the lifetime of this object (though perfectly safe) will likely lead to the logs not happenning. To reiterate: trying to do logging outside of the lifetime of a `jalog::Instance` is safe. There will be no crash and no C++ undefined behavior will be involved, however the logs in this case will in most cases be equivalent to `if (false)`.

`jalog::Instance::setup` returns a `jalog::Instance::SetupDSL` which itself has these basic setup methods: 

* `defaultLevel(jalog::Level)` - set the default level for the logger. This is relevant for newly created scopes (more on scope creation [here](creating-scopes.md)).
* `template <T, ...Args> add(Args&&...)` - add a sink by type. This call forwards its arguments the arguments to the sink's constructor.
* add(std::shared_ptr<Sink> sink

The setup DSL object will do the setup in its destructor, and this is the preferred use:

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
    setup.add(mysink)
    setup.defaultLevel(jalog::Level::Info);
}
```


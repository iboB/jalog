# Creating Scopes

Jalog supports multiple scopes. Scopes are just instances of `jalog::Scope` so once you create an instance of it, you have a scope. To do logging using your scope, you must give it as an argument to the logging function or macro. The `_SCOPE` macros are created for that purpose.

Here a trivial example.

```c++
jalog::Scope myscope("myscope");

JALOG_SCOPE(myscope, Info, "Logging in 'myscope'");
```

The macros with don't have a `_SCOPE` suffix also internally expand to the `_SCOPE` ones. They just provide the Jalog's default scope as a first argument. Jalog's default scope - `jalog::Default_Scope` is declared in `jalog/DefaultScope.hpp` and is just a global instance of `jalog::Scope` which is instantiated with the library for convenience. So, if you want to be extra explicit you can do default-scope logging like this:

```c++
JALOG_SCOPE(jalog::Default_Scope, Info, "Logging in Jalog's default scope");
```

## Defining Scopes

The constructor of `jalog::Scope` takes three arguments:

* `Scope(std::string_view label, uintptr_t id = 0, intptr_t userData = -1)`
    * `label` - the label of the scope which is to appear in your log messages when you view them. Using the same label for multiple scopes is safe (though it will likely be confusing when you read logs).
    * `id` and `data` - which are user provided arguments with default values. You can make use of them in custom sinks. Jalog stores them, but doesn't use them in any way. See [Custom Sinks](custom-sinks.md) about creating custom sinks.


### Scopes of Scopes

In the trivial example from above the scope's... er... well... scope was the same as the logging function. Oviously if you define a scope in a function and want to use it another, you should pass it as an argument of the second one. This is not very conventient, so to make them easier to use, scopes are best defined globally (or, perhaps as members in some singleton or singleton-like class).

Note that if you define a scope globally to be used in multiple compilation units (.cpp files), you should take care to separte the definition and the declaration, much like the case is with `jalog::Default_Scope`. Here is an example

```c++
// myscope.hpp
#pragma once
#include <jalog/Scope.hpp>
extern jalog::Scope myscope;
```

```c++
// myscope.cpp
#include "myscope.hpp"
jalog::Scope myscope("myscope");
```

Then you are free to include `myscope.hpp` in any file you want and do logging in it.

The lifetime of scopes is in no way bound to the lifetime of `jalog::Instance` which we introduced in the [Basics tutorial](basics.md). You are free to define scopes globally or locally regardless of the your `jalog::Instance`'s lifetime.

## Managing Scopes

As mentioned in the [Basics tutorial](basics.md) you can set a default log level for all scopes when you setup you Jalog instance. This is only meant for newly created scopes. A scope created before setting up your Jalog instance, will have its level set to `Off`. Once the instance is setup, the levels of all living scopes will be set to te instance's default level. Any scope created after the instance's setup will have its level set to the instance's default level.

Once the instance has been setup you can individually set the level of each scope using `setLevel`. For example:

```c++
myscope.setLevel(jalog::Level::Error); // only log errors and criticals here
jalog::Default_Scope.setLevel(jalog::Level::Off); // disable this entirely
```

You can get a scope's level with `.level()` and you can check whether a scope is enabled for a particular level with `.enabled(level)`.

To get the arguments you used in the scope's creation, you can use `.desc()` which will return a `jalog::ScopeDesc` structure, which has members for `label`, `id`, and `userData`.

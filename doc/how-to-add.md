# Adding Jalog to Your Project

Note that Jalog requires at least C++17, including for the projects which use it.

## Ways to add

Here are several ways by which you can add Jalog to your project. All of them require [CMake](https://cmake.org/). Support for other build systems/generators is not planned.

The CMake script defines the target `jalog::jalog` which you can add as a link library to your own targets.

### As a CPM.cmake package

The easiest way to add Jalog to a project would be as a [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) package. If you are using this package manager (and you should be :) ), you only need to add this line to your `CMakeLists.txt`: `CPMAddPackage(gh:iboB/jalog@0.1.0)`. *Update the version "0.1.0" to the one you want.*

Currently this is the only supported way of adding Jalog to a project though you can also add it:

### As a submodule/subrepo

Jalog uses [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) to handle its own depenendcies. If you add Jalog as a submodule to your repo and `add_subdirectory` things will likely still work. Jalog's CMake configuration will run CPM.cmake and fetch the dependencies. However if your project uses any of these dependencies without CPM.cmake there will be a clash of targets and the project likely won't build (or worse yet, it will build but will contain strange bugs due to ODR violations and ABI differences)

The dependencies of the project are

* [iboB/icm@1.5.4](https://github.com/iboB/icm)
* [iboB/splat@1.3.3](https://github.com/iboB/splat)
* [iboB/itlib@1.11.5](https://github.com/iboB/itlib)
* [iboB/mscharconv@1.2.3](https://github.com/iboB/mscharconv)
* (for building the tests only) [iboB/doctest-util@0.1.3](https://github.com/iboB/doctest-util)

### Other ways

If you provide the targets of the dependencies and only use `code/jalog/CMakeLists.txt` things will likely work.

As a whole the build of Jalog is pretty straight-forward, so creating a script for another build system will likely be easy.

## Configuration

Jalog's CMakeLists offers several configuration options:

* `JALOG_STATIC` - Whether to build a static library. Off by default, leading to a shared library. It's highly recommended to build Jalog as a shared library if there are multiple shared libraries which use Jalog in your project. The build scripts respect `ICM_STATIC_LIBS` from [icm_add_lib](https://github.com/iboB/icm/blob/master/icm_add_lib.cmake)
* `JALOG_NO_BUILTIN_ASYNC` - Remove support for the built-in async logging features of the library. Note that async logging is still possible with this, but it has to be user-defined.
* `JALOG_BUILD_SINKLIB` - whether to also build the [Jalog.Sinklib](sinklib.md) extension which provides several sinks (CMake target `jalog::sinklib`). On by default
* `JALOG_BUILD_TESTS` - whether to also build the Jalog unit tests. Off by default
* `JALOG_BUILD_EXAMPLES` - whether to also build the Jalog examples. Off by default

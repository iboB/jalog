* Think about replacing `operator,` with function calls. Using `operator,` is unpleasant. This may mean using arity macros, but <10 args should be fine, no?
* Think about structured logging - an event id, so logs can be machine analyzed. This can be solved by using the level field in a clever way (perhaps level orred with something). Example LOG(SESSION_JOIN) can be collected by a sink which counts active sessions
* `if constexpr` for compile-time disabled log levels
    * Custom types for scopes derived from `Scope` with `constexpr Level minLevel` for the check
    * Macros `JALOG_DECLARE_SCOPE`, `JALOG_DEFINE_SCOPE` for such cases
* Perhaps add `LevelStream(scope, level)` and `Stream(scope)` to avoid cases where scope/level is disabled at compile time and one outputs multiple levels
* Think of a good way to do padding

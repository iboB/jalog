* Think about structured logging - an event id, so logs can be machine analyzed. This can be solved by using the level field in a clever way (perhaps level orred with something). Example LOG(SESSION_JOIN) can be collected by a sink which counts active sessions
* `if constexpr` for compile-time disabled log levels
* What to do with disabled streams? Perhaps `OptionalStream`?
* Think of a good way to do padding

// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <splat/inline.h>
#include <memory>

namespace jalog::qwrite {

struct q_char_alloc : public std::allocator<char> {
    using super = std::allocator<char>;
    using value_type = super::value_type;
    using size_type = super::size_type;
    using difference_type = super::difference_type;
    using propagate_on_container_move_assignment = super::propagate_on_container_move_assignment;

    // create special noop construct/destroy funcs
    template <typename... Args>
    FORCE_INLINE void construct(char*, Args&&...) {}
    template <typename... Args>
    FORCE_INLINE void destroy(char*) {}
};

} // namespace jalog::qwrite

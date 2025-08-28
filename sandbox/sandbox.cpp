// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <jalog/Log.hpp>

namespace math {

struct vec { int x, y; };

std::ostream& operator<<(std::ostream& o, const vec& v) {
    return o << v.x << ' ' << v.y;
}

jalog::BasicStream& operator,(jalog::BasicStream& s, const vec& v) {
    return s, '(', v.x, ';', v.y, ')';
}

}

int main() {
    JALOG(Info, "Hello ", sizeof(jalog::ScopeDesc));

    math::vec v = {1, 2};
    JALOG(Info, "vec: ", v);

    return 0;
}

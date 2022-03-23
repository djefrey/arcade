#pragma once

namespace utils {
    template <auto Function>
    struct DeleterFromFunction {
        template <typename Type>
        constexpr void operator()(Type *arg) const {
            Function(arg);
        };
    };
};

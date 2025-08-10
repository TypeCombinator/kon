// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef SCOPE_3404B22B_141A_401E_9871_5B2BFE8B6CA8
#define SCOPE_3404B22B_141A_401E_9871_5B2BFE8B6CA8
#include <type_traits>
#include <utility>

namespace kon {

template <typename EF>
class [[nodiscard]] scope_exit {
   public:
    template <typename EFP>
    explicit constexpr scope_exit(EFP&& ef) noexcept(std::is_nothrow_constructible_v<EF, EFP>)
        requires(!std::is_same_v<std::remove_cvref_t<EFP>, scope_exit>)
             && std::is_constructible_v<EF, EFP>
        : exit_function(std::forward<EFP>(ef)) {
    }

    constexpr scope_exit(scope_exit&& other) noexcept(std::is_nothrow_move_constructible_v<EF>)
        : exit_function(std::move<EF>(other.exit_function)) {
        other.dismiss();
    }

    scope_exit(const scope_exit&) = delete;
    scope_exit& operator=(const scope_exit&) = delete;
    scope_exit& operator=(scope_exit&&) = delete;

    constexpr ~scope_exit() {
        if (engaged) {
            // TODO: Handle exception throwing.
            exit_function();
        }
    }

    constexpr void dismiss() noexcept {
        engaged = false;
    }

    constexpr void rehire() noexcept {
        engaged = true;
    }

   private:
    [[no_unique_address]]
    EF exit_function;
    bool engaged = true;
};

template <typename EF>
scope_exit(EF) -> scope_exit<EF>;

} // namespace kon
#endif /* scope.hpp */
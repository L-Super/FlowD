//
// Created by LMR on 25-5-7.
//

#pragma once
/**
 * The `defer` macro provides a mechanism similar to Go's `defer` statement.
 * example 1:
{
    FILE* file = fopen("example.txt", "r");
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }
    defer(fclose(file)); // Ensures the file is closed when the scope exits

    // Perform operations on the file
}

 * example 2:
class A {
public:
    ~A()
    {
        std::cout << "~A()\n";
    }
};

int main(int argc, char** argv)
{

    {
        A* a = new A();
        // do something...
        defer(delete a);
    }
    return 0;
}
 */

#include <type_traits>
#include <utility>

namespace detail {

template<typename F>
struct Defer {
    Defer(F&& f) noexcept : _f(std::forward<F>(f)) {}
    ~Defer()
    {
        _f();
    }
    typename std::remove_reference<F>::type _f;
};

template<typename F>
inline Defer<F> make_defer(F&& f) noexcept
{
    return Defer<F>(std::forward<F>(f));
}

#define _defer_concat(x, n) x##n
#define _defer_make_name(x, n) _defer_concat(x, n)
#define _defer_name _defer_make_name(_defer_var, __LINE__)

} // namespace detail

#define defer(e)                                                                                                       \
    auto _defer_name = detail::make_defer([&]() {                                                                      \
        e;                                                                                                             \
    })


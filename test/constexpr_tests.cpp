#include <array>
#include <utility>

#include <catch2/catch.hpp>
#include "lambda_coroutines/lambda_coroutines.hpp"


// need our own constexpr exchange because that only
// comes with C++20
template<class T, class U = T>
constexpr T
  exchange(T &obj, U &&new_value)
{
  T old_value = std::move(obj);
  obj = std::forward<U>(new_value);
  return old_value;
}

constexpr std::array<unsigned long long, 5> get_first_5()
{
  auto fib = [state = 0, fib_2 = 0ULL, fib_1 = 1ULL]() mutable -> std::optional<unsigned long long> {
    lambda_co_begin(state);

    lambda_co_yield(0);
    lambda_co_yield(1);

    while (fib_1 < std::numeric_limits<decltype(fib_1)>::max() / 2) {
      fib_2 = exchange(fib_1, fib_2 + fib_1);
      lambda_co_yield(fib_1);
    }

    lambda_co_return({});
  };

  return { fib().value(), fib().value(), fib().value(), fib().value(), fib().value() };
}

TEST_CASE("lambda_coroutines are constexpr capable", "[lambda_coroutines]")
{
  STATIC_REQUIRE(get_first_5()[4] == 3ULL);
}

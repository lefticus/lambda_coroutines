#include <functional>
#include <iostream>

#include "lambda_coroutines/lambda_coroutines.hpp"
#include <docopt/docopt.h>
#include <fmt/format.h>

static constexpr auto USAGE =
  R"(Lambda Coroutines Test
   Usage:
          lambda_coroutines [options]

 Options:
          -h --help     Show this screen.
          --version     Show version.
          --stride=<X>  Stepping between fib numbers [default: 1].
)";

int main(int argc, const char **argv)
{
  std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
    { std::next(argv), std::next(argv, argc) },
    true,// show help if requested
    "Lambda Coroutines 0.1");// version string


  const auto stride = static_cast<std::size_t>(args["--stride"].asLong());

  // generates a new fib each time called, no concern with overflow
  auto fib = [state = 0, fib_2 = 0u, fib_1 = 1u]() mutable -> unsigned int {
    lambda_co_begin(state);

    lambda_co_yield(0);
    lambda_co_yield(1);

    while (true) {
      fib_2 = std::exchange(fib_1, fib_2 + fib_1);
      lambda_co_yield(fib_1);
    }

    lambda_co_return(0);
  };

  // generates the set of all fibonacci numbers representable by a ull, returns
  // empty optional at end of list
  auto fib2 = [state = 0, fib_2 = 0ULL, fib_1 = 1ULL]() mutable -> std::optional<unsigned long long> {
    lambda_co_begin(state);

    lambda_co_yield(0);
    lambda_co_yield(1);

    while (fib_1 < std::numeric_limits<decltype(fib_1)>::max() / 2) {
      fib_2 = std::exchange(fib_1, fib_2 + fib_1);
      lambda_co_yield(fib_1);
    }

    lambda_co_return({});
  };

  fmt::print("Requested Range of Fib Numbers:\n");
  for (const auto value : lambda_coroutines::range(fib, 0, 15, stride)) {
    fmt::print("{}\n", value);
  }

  fmt::print("All possible Fib numbers representable by 'unsigned long long'");
  for (const auto value : lambda_coroutines::while_has_value(fib2)) {
    fmt::print("{}\n", value);
  }
}

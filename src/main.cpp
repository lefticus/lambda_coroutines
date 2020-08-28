#include <functional>
#include <iostream>

#include <spdlog/spdlog.h>


#include <docopt/docopt.h>

#include <iostream>

static constexpr auto USAGE =
  R"(Naval Fate.

    Usage:
          naval_fate ship new <name>...
          naval_fate ship <name> move <x> <y> [--speed=<kn>]
          naval_fate ship shoot <x> <y>
          naval_fate mine (set|remove) <x> <y> [--moored | --drifting]
          naval_fate (-h | --help)
          naval_fate --version
 Options:
          -h --help     Show this screen.
          --version     Show version.
          --speed=<kn>  Speed in knots [default: 10].
          --moored      Moored (anchored) mine.
          --drifting    Drifting mine.
)";

int main(int argc, const char **argv)
{
  std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
    { std::next(argv), std::next(argv, argc) },
    true,// show help if requested
    "Naval Fate 2.0");// version string

  for (auto const &arg : args) {
    std::cout << arg.first << arg.second << std::endl;
  }

    auto fib = [state = 0, fib_2 = 0, fib_1 = 1]() mutable -> int {
    lambda_co_begin(state);

    lambda_co_yield(0);
    lambda_co_yield(1);

    while (true) {
      fib_2 = std::exchange(fib_1, fib_2 + fib_1);
      lambda_co_yield(fib_1);
    }

    lambda_co_return(-1);
  };

  // generates the set of all fibonacci numbers representable by a ull
  auto fib2 = [state = 0, fib_2 = 0ull,
               fib_1 = 1ull]() mutable -> std::optional<unsigned long long> {
    lambda_co_begin(state);

    lambda_co_yield(0);
    lambda_co_yield(1);

    while (fib_1 < std::numeric_limits<decltype(fib_1)>::max() / 2) {
      fib_2 = std::exchange(fib_1, fib_2 + fib_1);
      lambda_co_yield(fib_1);
    }

    lambda_co_return({});
  };

  //  for (const auto value : lambda_co_range(fib, 5, 5, 2))
  //  {
  //      fmt::print("{}\n", value);
  //  }

  for (const auto value : lambda_co_while_not_empty(fib2)) {
    fmt::print("{}\n", value);
  }

  //Use the default logger (stdout, multi-threaded, colored)
  spdlog::info("Hello, {}!", "World");

  fmt::print("Hello, from {}\n", "{fmt}");
}

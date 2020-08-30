# Lambda Coroutines

[![codecov](https://codecov.io/gh/lefticus/lambda_coroutines/branch/master/graph/badge.svg)](https://codecov.io/gh/lefticus/lambda_coroutines)
![CMake](https://github.com/lefticus/lambda_coroutines/workflows/CMake/badge.svg)

## Description

A lightweight macro-based coroutine / resumable / cooperative multitasking function utility designed for C++14 lambdas.

# Examples

## Rotating Infinite Sequence

```cpp
// lambda must be mutable
auto next_direction = [state=0]() mutable {
  // set up coroutine, needs a captured integral `state` value
  lambda_co_begin(state);

  // for eternity yeild the next possible value
  while (true) {
    lambda_co_yield(directions::Left);
    lambda_co_yield(directions::Right);
    lambda_co_yield(directions::Up);
    lambda_co_yield(directions::Down);
  }

  lambda_co_end();
};

int main() {
  auto val1 = next_direction();  // returns Left
  auto val2 = next_direction();  // returns Right
  // etc
}
```

## Obligatory Fibonacci Sequence

```cpp
// generates the set of all fibonacci numbers representable by a ull, returns
// empty optional at end of list
auto fib = [state = 0, fib_2 = 0ULL, fib_1 = 1ULL]() mutable -> std::optional<unsigned long long> {
  lambda_co_begin(state);

  lambda_co_yield(0);
  lambda_co_yield(1);

  while (fib_1 < std::numeric_limits<decltype(fib_1)>::max() / 2) {
    fib_2 = std::exchange(fib_1, fib_2 + fib_1);
    lambda_co_yield(fib_1);
  }

  lambda_co_return({});
};
```

## Ranged `for` Statement With Coroutine

```cpp
// Using the Obligatory Fibonacci Sequence
fmt::print("All possible Fib numbers representable by 'unsigned long long'");
for (const auto value : lambda_coroutines::while_has_value(fib)) {
  fmt::print("{}\n", value);
}
```

## CPU Instruction Decoding State Machine

[Compiler Explorer playground](https://godbolt.org/z/7dr8j7) for this example.

```cpp
enum OpCodes : std::uint8_t {
  ADD = 0,
  STA = 1,
  NOP = 2
};
struct Machine {
  std::uint8_t PC{0};
  std::uint8_t A{0};
  std::array<uint8_t, 256> RAM{STA, 10, ADD, 15};
};

Machine machine;

auto CPU = [state = 0, &machine, op = OpCodes::NOP]() mutable {
  lambda_co_begin(state);

  while(true) {
    op = static_cast<OpCodes>(machine.RAM[machine.PC]);
    ++machine.PC;
    if (op == OpCodes::STA) {
      lambda_co_yield();
      machine.A = machine.RAM[machine.PC++];
      lambda_co_yield();
    } else if (op == OpCodes::ADD) {
      lambda_co_yield();
      machine.A += machine.RAM[machine.PC++];
      lambda_co_yield();
    } else if (op == OpCodes::NOP) {
      lambda_co_yield();
    };
  }

  lambda_co_end();
};
```



# Limitations

 * Cannot use `switch` statements in coroutine code
 * Cannot declare variables in coroutine code
 * Anything you care about must be part of the lambda capture declaration
 
 
# Using In Your Project

It is one VERY simple header file https://github.com/lefticus/lambda_coroutines/blob/main/include/lambda_coroutines/lambda_coroutines.hpp

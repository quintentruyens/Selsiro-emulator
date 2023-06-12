#ifndef SELSIRO_EMULATOR_SAMPLE_LIBRARY_HPP
#define SELSIRO_EMULATOR_SAMPLE_LIBRARY_HPP

#include <Selsiro_emulator/sample_library_export.hpp>

[[nodiscard]] SAMPLE_LIBRARY_EXPORT int factorial(int input) noexcept;

// NOLINTNEXTLINE(misc-no-recursion)
[[nodiscard]] constexpr int factorial_constexpr(int input) noexcept {
  if (input == 0) {
    return 1;
  }

  return input * factorial_constexpr(input - 1);
}

#endif /* SELSIRO_EMULATOR_SAMPLE_LIBRARY_HPP */

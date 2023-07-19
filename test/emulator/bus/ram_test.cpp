#include "emulator/bus/ram.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("RAM is zero initialized", "[emulator][bus][ram]") {
  static constexpr selsiro_emulator::RAM<16, 24> ram{};

  STATIC_REQUIRE(ram.read_word(16) == 0);
  STATIC_REQUIRE(ram.read_word(20) == 0);
  STATIC_REQUIRE(ram.read_word(24) == 0);
  STATIC_REQUIRE(ram.read_word(28) == 0);
  STATIC_REQUIRE(ram.read_word(32) == 0);
  STATIC_REQUIRE(ram.read_word(36) == 0);
}

TEST_CASE("RAM is inaccessible outside of range", "[emulator][bus][ram]") {
  using BusFault = selsiro_emulator::BusFault;
  static constexpr selsiro_emulator::RAM<16, 24> ram{};

  STATIC_REQUIRE_FALSE(ram.read_word(0).has_value());
  STATIC_REQUIRE(ram.read_word(0).error() == BusFault::AccessFault);

  // Aligned address before start of RAM
  STATIC_REQUIRE_FALSE(ram.read_word(12).has_value());
  STATIC_REQUIRE(ram.read_word(12).error() == BusFault::AccessFault);

  // Misaligned address before start of RAM
  STATIC_REQUIRE_FALSE(ram.read_word(15).has_value());
  STATIC_REQUIRE(ram.read_word(15).error() == BusFault::AccessFault);

  // Aligned address after end of RAM
  STATIC_REQUIRE_FALSE(ram.read_word(40).has_value());
  STATIC_REQUIRE(ram.read_word(40).error() == BusFault::AccessFault);

  // Misaligned address after end of RAM
  STATIC_REQUIRE_FALSE(ram.read_word(41).has_value());
  STATIC_REQUIRE(ram.read_word(41).error() == BusFault::AccessFault);
}

TEST_CASE("RAM is inaccessible at misaligned addresses",
          "[emulator][bus][ram]") {
  using BusFault = selsiro_emulator::BusFault;
  static constexpr selsiro_emulator::RAM<16, 24> ram{};

  STATIC_REQUIRE_FALSE(ram.read_word(17).has_value());
  STATIC_REQUIRE(ram.read_word(17).error() == BusFault::AddressMisaligned);

  STATIC_REQUIRE_FALSE(ram.read_word(18).has_value());
  STATIC_REQUIRE(ram.read_word(18).error() == BusFault::AddressMisaligned);

  STATIC_REQUIRE_FALSE(ram.read_word(19).has_value());
  STATIC_REQUIRE(ram.read_word(19).error() == BusFault::AddressMisaligned);

  STATIC_REQUIRE_FALSE(ram.read_word(25).has_value());
  STATIC_REQUIRE(ram.read_word(25).error() == BusFault::AddressMisaligned);

  STATIC_REQUIRE_FALSE(ram.read_word(37).has_value());
  STATIC_REQUIRE(ram.read_word(37).error() == BusFault::AddressMisaligned);

  STATIC_REQUIRE_FALSE(ram.read_word(38).has_value());
  STATIC_REQUIRE(ram.read_word(38).error() == BusFault::AddressMisaligned);

  STATIC_REQUIRE_FALSE(ram.read_word(39).has_value());
  STATIC_REQUIRE(ram.read_word(39).error() == BusFault::AddressMisaligned);
}

consteval selsiro_emulator::RAM<16, 24> initialized_ram(auto &&vals) {
  selsiro_emulator::RAM<16, 24> ram{};
  ram.fill(std::begin(vals), std::end(vals));
  return ram;
}

TEST_CASE("RAM can be initialized", "[emulator][bus][ram]") {
  static constexpr auto ram =
      initialized_ram(std::array<selsiro_emulator::Word, 6>{1, 2, 3, 4, 5, 6});

  STATIC_REQUIRE_FALSE(ram.read_word(12).has_value());
  STATIC_REQUIRE(ram.read_word(16) == 1);
  STATIC_REQUIRE(ram.read_word(20) == 2);
  STATIC_REQUIRE(ram.read_word(24) == 3);
  STATIC_REQUIRE(ram.read_word(28) == 4);
  STATIC_REQUIRE(ram.read_word(32) == 5);
  STATIC_REQUIRE(ram.read_word(36) == 6);
  STATIC_REQUIRE_FALSE(ram.read_word(40).has_value());
}

TEST_CASE("RAM can be initialized with an array that is too small",
          "[emulator][bus][ram]") {
  static constexpr auto ram =
      initialized_ram(std::array<selsiro_emulator::Word, 3>{1, 2, 3});

  // First 3 addresses initialized, rest set to zero
  STATIC_REQUIRE_FALSE(ram.read_word(12).has_value());
  STATIC_REQUIRE(ram.read_word(16) == 1);
  STATIC_REQUIRE(ram.read_word(20) == 2);
  STATIC_REQUIRE(ram.read_word(24) == 3);
  STATIC_REQUIRE(ram.read_word(28) == 0);
  STATIC_REQUIRE(ram.read_word(32) == 0);
  STATIC_REQUIRE(ram.read_word(36) == 0);
  STATIC_REQUIRE_FALSE(ram.read_word(40).has_value());
}

TEST_CASE("RAM can be initialized with an array that is too large",
          "[emulator][bus][ram]") {
  static constexpr auto ram = initialized_ram(
      std::array<selsiro_emulator::Word, 9>{1, 2, 3, 4, 5, 6, 7, 8, 9});

  STATIC_REQUIRE_FALSE(ram.read_word(12).has_value());
  STATIC_REQUIRE(ram.read_word(16) == 1);
  STATIC_REQUIRE(ram.read_word(20) == 2);
  STATIC_REQUIRE(ram.read_word(24) == 3);
  STATIC_REQUIRE(ram.read_word(28) == 4);
  STATIC_REQUIRE(ram.read_word(32) == 5);
  STATIC_REQUIRE(ram.read_word(36) == 6);
  STATIC_REQUIRE_FALSE(ram.read_word(40).has_value());
}

#include "emulator/registers.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Registers are zero initialized", "[emulator][registers]") {
  static constexpr selsiro_emulator::Registers regs{};

  STATIC_REQUIRE(regs.read(0).has_value());
  STATIC_REQUIRE(regs.read(0).value() == 0U);

  STATIC_REQUIRE(regs.read(1).has_value());
  STATIC_REQUIRE(regs.read(1).value() == 0U);

  STATIC_REQUIRE(regs.read(5).has_value());
  STATIC_REQUIRE(regs.read(5).value() == 0U);

  STATIC_REQUIRE(regs.read(17).has_value());
  STATIC_REQUIRE(regs.read(17).value() == 0U);

  STATIC_REQUIRE(regs.read(31).has_value());
  STATIC_REQUIRE(regs.read(31).value() == 0U);
}

TEST_CASE("Registers above 31 are inaccessible", "[emulator][registers]") {
  static constexpr selsiro_emulator::Registers regs{};

  STATIC_REQUIRE_FALSE(regs.read(32).has_value());
  STATIC_REQUIRE_FALSE(regs.read(33).has_value());
  STATIC_REQUIRE_FALSE(regs.read(50).has_value());
  STATIC_REQUIRE_FALSE(regs.read(128).has_value());
  STATIC_REQUIRE_FALSE(regs.read(55194).has_value());
}

consteval selsiro_emulator::Registers
written_registers(unsigned int index, selsiro_emulator::Word data) {
  selsiro_emulator::Registers regs{};
  regs.write(index, data);
  return regs;
}

TEST_CASE("Registers can be written", "[emulator][registers]") {
  static constexpr selsiro_emulator::Registers regs =
      written_registers(/*index=*/5, /*data=*/185904);

  STATIC_REQUIRE(regs.read(5).has_value());
  STATIC_REQUIRE(regs.read(5).value() == 185904U);
}

TEST_CASE("Register 0 cannot be overwritten", "[emulator][registers]") {
  static constexpr selsiro_emulator::Registers regs =
      written_registers(/*index=*/0, /*data=*/185904);

  STATIC_REQUIRE(regs.read(0).has_value());
  STATIC_REQUIRE(regs.read(0).value() == 0U);
}

TEST_CASE("Register writes above 31 are ignored", "[emulator][registers]") {
  static constexpr selsiro_emulator::Registers regs =
      written_registers(/*index=*/32, /*data=*/185904);

  STATIC_REQUIRE(regs.read(0).has_value());
  STATIC_REQUIRE(regs.read(0).value() == 0U);

  STATIC_REQUIRE(regs.read(1).has_value());
  STATIC_REQUIRE(regs.read(1).value() == 0U);

  STATIC_REQUIRE(regs.read(5).has_value());
  STATIC_REQUIRE(regs.read(5).value() == 0U);

  STATIC_REQUIRE(regs.read(17).has_value());
  STATIC_REQUIRE(regs.read(17).value() == 0U);

  STATIC_REQUIRE(regs.read(31).has_value());
  STATIC_REQUIRE(regs.read(31).value() == 0U);
}

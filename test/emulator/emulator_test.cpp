#include "emulator/emulator.hpp"

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <cstdint>

TEST_CASE("Emulator is initialized correctly", "[emulator]") {
  static constexpr std::array<selsiro_emulator::Word, 6> program{
      1, 5, 7, 8, 0xFFFFFFFF, 584718};
  const selsiro_emulator::Emulator emulator{std::begin(program),
                                            std::end(program)};

  REQUIRE(emulator.pc() == 0);

  REQUIRE(emulator.registers().read(0) == 0);
  REQUIRE(emulator.registers().read(1) == 0);
  REQUIRE(emulator.registers().read(17) == 0);
  REQUIRE(emulator.registers().read(31) == 0);

  REQUIRE(emulator.ram().read_word(0) == 1U);
  REQUIRE(emulator.ram().read_word(4) == 5U);
  REQUIRE(emulator.ram().read_word(8) == 7U);
  REQUIRE(emulator.ram().read_word(12) == 8U);
  REQUIRE(emulator.ram().read_word(16) == 0xFFFFFFFFU);
  REQUIRE(emulator.ram().read_word(20) == 584718U);
  REQUIRE(emulator.ram().read_word(24) == 0U);
  REQUIRE(emulator.ram().read_word(32) == 0U);
  REQUIRE(emulator.ram().read_word(0x3FFFFFC) == 0U);
}

TEST_CASE("Emulator is clocked correctly", "[emulator]") {
  // addi x1, x0, 1
  static constexpr std::array<selsiro_emulator::Word, 1> program{0x00100093};
  selsiro_emulator::Emulator emulator{std::begin(program), std::end(program)};

  REQUIRE(emulator.pc() == 0);

  REQUIRE(emulator.registers().read(0) == 0);
  REQUIRE(emulator.registers().read(1) == 0);
  REQUIRE(emulator.registers().read(17) == 0);
  REQUIRE(emulator.registers().read(31) == 0);

  REQUIRE(emulator.ram().read_word(0) == 0x00100093U);
  REQUIRE(emulator.ram().read_word(4) == 0U);
  REQUIRE(emulator.ram().read_word(8) == 0U);
  REQUIRE(emulator.ram().read_word(0x3FFFFFC) == 0U);

  emulator.clock();

  REQUIRE(emulator.registers().read(1) == 1);
  REQUIRE(emulator.pc() == 4);

  REQUIRE(emulator.registers().read(0) == 0);
  REQUIRE(emulator.registers().read(2) == 0);
  REQUIRE(emulator.registers().read(17) == 0);
  REQUIRE(emulator.registers().read(31) == 0);
}

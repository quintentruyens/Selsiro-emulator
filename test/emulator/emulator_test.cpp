#include "emulator/emulator.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Emulator is initialized correctly", "[emulator]") {
  static constexpr std::array<selsiro_emulator::Word, 6> arr{
      1, 5, 7, 8, 0xFFFFFFFF, 584718};
  static constexpr selsiro_emulator::Emulator emulator{std::begin(arr),
                                                       std::end(arr)};

  STATIC_REQUIRE(emulator.pc() == 0);

  STATIC_REQUIRE(emulator.registers().read(0) == 0);
  STATIC_REQUIRE(emulator.registers().read(1) == 0);
  STATIC_REQUIRE(emulator.registers().read(17) == 0);
  STATIC_REQUIRE(emulator.registers().read(31) == 0);

  STATIC_REQUIRE(emulator.ram().read_word(0) == 1);
  STATIC_REQUIRE(emulator.ram().read_word(4) == 5);
  STATIC_REQUIRE(emulator.ram().read_word(8) == 7);
  STATIC_REQUIRE(emulator.ram().read_word(12) == 8);
  STATIC_REQUIRE(emulator.ram().read_word(16) == 0xFFFFFFFF);
  STATIC_REQUIRE(emulator.ram().read_word(20) == 584718);
  STATIC_REQUIRE(emulator.ram().read_word(24) == 0);
  STATIC_REQUIRE(emulator.ram().read_word(32) == 0);
  STATIC_REQUIRE(emulator.ram().read_word(0x3FFFFFC) == 0);
}

consteval selsiro_emulator::Emulator
clocked_emulator(auto &&memory, std::uint32_t cycles_to_clock) {
  selsiro_emulator::Emulator emulator{std::begin(memory), std::end(memory)};
  for (std::uint32_t i = 0; i < cycles_to_clock; ++i) {
    emulator.clock();
  }
  return emulator;
}

TEST_CASE("Emulator is clocked correctly", "[emulator]") {
  // addi x1, x0, 1
  static constexpr auto emulator = clocked_emulator(
      std::array<selsiro_emulator::Word, 1>{0x00100093}, /*cycles_to_clock=*/1);

  STATIC_REQUIRE(emulator.registers().read(1) == 1);
  STATIC_REQUIRE(emulator.pc() == 1);

  STATIC_REQUIRE(emulator.registers().read(0) == 0);
  STATIC_REQUIRE(emulator.registers().read(2) == 0);
  STATIC_REQUIRE(emulator.registers().read(17) == 0);
  STATIC_REQUIRE(emulator.registers().read(31) == 0);
}

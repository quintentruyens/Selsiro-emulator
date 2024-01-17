#ifndef EMULATOR_EMULATOR_HPP
#define EMULATOR_EMULATOR_HPP

#include "bus/ram.hpp"
#include "instruction.hpp"
#include "registers.hpp"
#include "util.hpp"

namespace selsiro_emulator {

/**
 * @brief Implements an emulation of a RISC-V system
 */
class Emulator {

public:
  template <std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
  constexpr Emulator(Iterator program_first, Sentinel program_last) noexcept {
    m_ram.fill(program_first, program_last);
  }

  /**
   * @brief Get the current program counter.
   *
   * @return The current program counter
   */
  [[nodiscard]] constexpr Word pc() const noexcept { return m_pc; }

  /**
   * @brief Get the current state of the registers.
   *
   * @return The current state of the registers
   */
  [[nodiscard]] constexpr const auto &registers() const noexcept {
    return m_registers;
  }

  /**
   * @brief Get the current state of the RAM.
   *
   * @return The current state of the RAM
   */
  [[nodiscard]] constexpr const auto &ram() const noexcept { return m_ram; }

  /**
   * @brief Advance the state of the CPU by one clock cycle.
   * This will execute one instruction.
   */
  constexpr void clock() noexcept {
    // Ignore bus errors for now until traps are implemented
    // TODO: Correctly handle bus errors once traps are implemented
    auto instr_word = m_ram.read_word(m_pc).value_or(0);
    const Instruction instr{instr_word};
    if (instr.opcode() != 0b0010011 || instr.funct3() != 0b000) {
      // Only addi instructions are currently implemented
      return;
    }

    // addi rd, rs1, imm_i
    auto rs1 = m_registers.read(instr.rs1());
    assert(rs1.has_value());
    auto result = rs1.value_or(0) + instr.imm_i();
    m_registers.write(instr.rd(), result);

    m_pc++;
  }

private:
  Word m_pc{0};
  Registers m_registers{};
  RAM<0, (1U << 24U) * 4> m_ram{};
};

} // namespace selsiro_emulator

#endif

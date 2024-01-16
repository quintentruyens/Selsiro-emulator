#ifndef EMULATOR_INSTRUCTION_HPP
#define EMULATOR_INSTRUCTION_HPP

#include "emulator/util.hpp"

#include <bit>
#include <limits>
#include <optional>
#include <type_traits>

namespace selsiro_emulator {

/**
 * @brief Implements the decoding of RISC-V instructions into their fields.
 * All of the relevant fields of an instruction can be accessed.
 */
class Instruction {

public:
  constexpr explicit Instruction(Word instr) noexcept : m_instr(instr) {}

  /**
   * @brief Get this instruction as a Word.
   *
   * @return Word representing this instruction
   */
  [[nodiscard]] constexpr Word word() const noexcept { return m_instr; }

  /**
   * @brief Get the value of the opcode field of this instruction.
   *
   * @return The opcode of this instruction
   */
  [[nodiscard]] constexpr Word opcode() const noexcept {
    return get_field(OPCODE_FIELD);
  }

  /**
   * @brief Get the value of the funct3 field of this instruction.
   * This will work even if this instruction is of a type without a funct3
   * field, which will return an unspecified value.
   *
   * @return The funct3 of this instruction
   */
  [[nodiscard]] constexpr Word funct3() const noexcept {
    return get_field(FUNCT3_FIELD);
  }

  /**
   * @brief Get the value of the funct7 field of this instruction.
   * This will work even if this instruction is of a type without a funct7
   * field, which will return an unspecified value.
   *
   * @return The funct7 of this instruction
   */
  [[nodiscard]] constexpr Word funct7() const noexcept {
    return get_field(FUNCT7_FIELD);
  }

  /**
   * @brief Get the value of the rd field of this instruction.
   * This will work even if this instruction is of a type without a destination
   * register, which will return an unspecified value.
   *
   * @return The destination register of this instruction
   */
  [[nodiscard]] constexpr Word rd() const noexcept {
    return get_field(RD_FIELD);
  }

  /**
   * @brief Get the value of the rs1 field of this instruction.
   * This will work even if this instruction is of a type without a source
   * register, which will return an unspecified value.
   *
   * @return The first source register of this instruction
   */
  [[nodiscard]] constexpr Word rs1() const noexcept {
    return get_field(RS1_FIELD);
  }

  /**
   * @brief Get the value of the rs2 field of this instruction.
   * This will work even if this instruction is of a type without a second
   * source register, which will return an unspecified value.
   *
   * @return The second source register of this instruction
   */
  [[nodiscard]] constexpr Word rs2() const noexcept {
    return get_field(RS2_FIELD);
  }

  /**
   * @brief Get the value of the immediate field of an I-type instruction.
   * This will work even if this is not an I-type instruction, which will return
   * an unspecified value.
   *
   * @return The immediate field of an I-type instruction
   */
  [[nodiscard]] constexpr Word imm_i() const noexcept {
    return get_signed_field(I_IMM_FIELD);
  }

  /**
   * @brief Get the value of the immediate field of an S-type instruction.
   * This will work even if this is not an S-type instruction, which will return
   * an unspecified value.
   *
   * @return The immediate field of an S-type instruction
   */
  [[nodiscard]] constexpr Word imm_s() const noexcept {
    return get_signed_field(S_IMM_FIELD_2) << 5U | get_field(S_IMM_FIELD_1);
  }

  /**
   * @brief Get the value of the immediate field of an B-type instruction.
   * This will work even if this is not an B-type instruction, which will return
   * an unspecified value.
   *
   * @return The immediate field of an B-type instruction
   */
  [[nodiscard]] constexpr Word imm_b() const noexcept {
    return get_signed_field(B_IMM_FIELD_4) << 12U |
           get_field(B_IMM_FIELD_3) << 11U | get_field(B_IMM_FIELD_2) << 5U |
           get_field(B_IMM_FIELD_1) << 1U;
  }

  /**
   * @brief Get the value of the immediate field of an U-type instruction.
   * This will work even if this is not an U-type instruction, which will return
   * an unspecified value.
   *
   * @return The immediate field of an U-type instruction
   */
  [[nodiscard]] constexpr Word imm_u() const noexcept {
    return get_field(U_IMM_FIELD) << 12U;
  }

  /**
   * @brief Get the value of the immediate field of an J-type instruction.
   * This will work even if this is not an J-type instruction, which will return
   * an unspecified value.
   *
   * @return The immediate field of an J-type instruction
   */
  [[nodiscard]] constexpr Word imm_j() const noexcept {
    return get_signed_field(J_IMM_FIELD_4) << 20U |
           get_field(J_IMM_FIELD_3) << 12U | get_field(J_IMM_FIELD_2) << 11U |
           get_field(J_IMM_FIELD_1) << 1U;
  }

private:
  Word m_instr;

  struct Field {
    Word offset;
    Word length;
  };

  [[nodiscard]] constexpr Word get_field(Field field) const noexcept {
    return (m_instr >> field.offset) & ((1U << field.length) - 1U);
  }

  [[nodiscard]] constexpr Word get_signed_field(Field field) const noexcept {
    using Signed = std::make_signed_t<Word>;

    const Word val = get_field(field);
    const Word shift = std::numeric_limits<Word>::digits - field.length;
    // Altough the behaviour was implementation-defined in previous versions,
    // since c++20 right shift on signed operands are defined to perform
    // arithmetic shift right. This allows the use of a left shift followed by
    // an arithmetic right shift to perform sign extension.
    // NOLINTNEXTLINE(hicpp-signed-bitwise)
    return std::bit_cast<Word>((std::bit_cast<Signed>(val << shift)) >> shift);
  }

  static constexpr Field OPCODE_FIELD = {0, 7};
  static constexpr Field FUNCT3_FIELD = {12, 3};
  static constexpr Field FUNCT7_FIELD = {25, 7};
  static constexpr Field RD_FIELD = {7, 5};
  static constexpr Field RS1_FIELD = {15, 5};
  static constexpr Field RS2_FIELD = {20, 5};

  static constexpr Field I_IMM_FIELD = {20, 12};

  static constexpr Field S_IMM_FIELD_1 = {7, 5};
  static constexpr Field S_IMM_FIELD_2 = {25, 7};

  static constexpr Field B_IMM_FIELD_1 = {8, 4};
  static constexpr Field B_IMM_FIELD_2 = {25, 6};
  static constexpr Field B_IMM_FIELD_3 = {7, 1};
  static constexpr Field B_IMM_FIELD_4 = {31, 1};

  static constexpr Field U_IMM_FIELD = {12, 20};

  static constexpr Field J_IMM_FIELD_1 = {21, 10};
  static constexpr Field J_IMM_FIELD_2 = {20, 1};
  static constexpr Field J_IMM_FIELD_3 = {12, 8};
  static constexpr Field J_IMM_FIELD_4 = {31, 1};
};

} // namespace selsiro_emulator

#endif

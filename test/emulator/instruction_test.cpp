#include "emulator/instruction.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Instructions correctly store their value",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the values of some arbitrary instructions
  // lui x1, 0x561A9
  STATIC_REQUIRE(Instruction{0x561a90b7}.word() == 0x561a90b7U);
  // auipc x20, 0x93E78
  STATIC_REQUIRE(Instruction{0x93e78a17}.word() == 0x93e78a17U);
  // bne x18, x0, start
  STATIC_REQUIRE(Instruction{0xfe091ce3}.word() == 0xfe091ce3U);
  // lh x24, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x68919c03}.word() == 0x68919c03U);
  // andi x5, x31, 0x78F
  STATIC_REQUIRE(Instruction{0x78fff293}.word() == 0x78fff293U);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.word() == 0x40000033U);
  // csrrwi x9, 0x50A, 0x5
  STATIC_REQUIRE(Instruction{0x50a2d4f3}.word() == 0x50a2d4f3U);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.word() == 0x03e53733U);
}

TEST_CASE("Instruction opcodes are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the opcodes of some arbitrary instructions
  // lui x1, 0x561A9
  STATIC_REQUIRE(Instruction{0x561a90b7}.opcode() == 0b0110111U);
  // auipc x20, 0x93E78
  STATIC_REQUIRE(Instruction{0x93e78a17}.opcode() == 0b0010111U);
  // bne x18, x0, start
  STATIC_REQUIRE(Instruction{0xfe091ce3}.opcode() == 0b1100011U);
  // lh x24, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x68919c03}.opcode() == 0b0000011U);
  // andi x5, x31, 0x78F
  STATIC_REQUIRE(Instruction{0x78fff293}.opcode() == 0b0010011U);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.opcode() == 0b0110011U);
  // csrrwi x9, 0x50A, 0x5
  STATIC_REQUIRE(Instruction{0x50a2d4f3}.opcode() == 0b1110011U);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.opcode() == 0b0110011U);
}

TEST_CASE("Instruction funct3 fields are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the funct3 fields of some instructions
  // bne x18, x0, start
  STATIC_REQUIRE(Instruction{0xfe091ce3}.funct3() == 0b001U);
  // lbu x24, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x6891cc03}.funct3() == 0b100U);
  // sw x23, 0x76C(x8)
  STATIC_REQUIRE(Instruction{0x77742623}.funct3() == 0b010U);
  // andi x5, x31, 0x78F
  STATIC_REQUIRE(Instruction{0x78fff293}.funct3() == 0b111U);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.funct3() == 0b000U);
  // csrrwi x9, 0x50A, 0x5
  STATIC_REQUIRE(Instruction{0x50a2d4f3}.funct3() == 0b101U);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.funct3() == 0b011U);
}

TEST_CASE("Instruction funct7 fields are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the funct7 fields of some instructions
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.funct7() == 0b0100000U);
  // xor x11, x3, x17
  STATIC_REQUIRE(Instruction{0x0111c5b3}.funct7() == 0b0000000U);
  // sltu x24, x16, x28
  STATIC_REQUIRE(Instruction{0x01c83c33}.funct7() == 0b0000000U);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.funct7() == 0b0000001U);
  // rem x9, x22, x1
  STATIC_REQUIRE(Instruction{0x021b64b3}.funct7() == 0b0000001U);

  // The funct7 field should also correspond to the top bits of a immediate
  // shift instruction, altough they are actually an I-type instruction
  // slli x9, x26, 21
  STATIC_REQUIRE(Instruction{0x015d1493}.funct7() == 0b0000000U);
  // srai x8, x4, 15
  STATIC_REQUIRE(Instruction{0x40f25413}.funct7() == 0b0100000U);
}

TEST_CASE("Instruction destination registers are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the destination registers of some instructions
  // lui x1, 0x561A9
  STATIC_REQUIRE(Instruction{0x561a90b7}.rd() == 0b00001U);
  // auipc x20, 0x93E78
  STATIC_REQUIRE(Instruction{0x93e78a17}.rd() == 0b10100U);
  // jal x31, 0x14EF62
  STATIC_REQUIRE(Instruction{0xf634efef}.rd() == 0b11111U);
  // lh x24, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x68919c03}.rd() == 0b11000U);
  // andi x5, x31, 0x78F
  STATIC_REQUIRE(Instruction{0x78fff293}.rd() == 0b00101U);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.rd() == 0b00000U);
  // csrrwi x9, 0x50A, 0x5
  STATIC_REQUIRE(Instruction{0x50a2d4f3}.rd() == 0b01001U);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.rd() == 0b01110U);
}

TEST_CASE("Instruction first source registers are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the first source registers of some instructions
  // bne x18, x0, start
  STATIC_REQUIRE(Instruction{0xfe091ce3}.rs1() == 0b10010U);
  // lbu x24, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x6891cc03}.rs1() == 0b00011U);
  // sw x23, 0x76C(x8)
  STATIC_REQUIRE(Instruction{0x77742623}.rs1() == 0b01000U);
  // andi x5, x31, 0x78F
  STATIC_REQUIRE(Instruction{0x78fff293}.rs1() == 0b11111U);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.rs1() == 0b00000U);
  // csrrwi x9, 0x50A, 0x5
  STATIC_REQUIRE(Instruction{0x50a2d4f3}.rs1() == 0b00101U);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.rs1() == 0b01010U);
}

TEST_CASE("Instruction second source registers are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the second source registers of some instructions
  // bne x18, x0, start
  STATIC_REQUIRE(Instruction{0xfe091ce3}.rs2() == 0b00000U);
  // bgeu x2, x25, 0x450
  STATIC_REQUIRE(Instruction{0x45917863}.rs2() == 0b11001U);
  // sw x23, 0x76C(x8)
  STATIC_REQUIRE(Instruction{0x77742623}.rs2() == 0b10111U);
  // sb x31, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x69f184a3}.rs2() == 0b11111U);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.rs2() == 0b00000U);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.rs2() == 0b11110U);
}

TEST_CASE("Immediates of I-type instructions are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the immediates of some I-type instructions
  // jalr x28, 0x000(x31)
  STATIC_REQUIRE(Instruction{0x000f8e67}.imm_i() == 0x000U);
  // lw x17, 0xFFF(x9)
  STATIC_REQUIRE(Instruction{0xfff4a883}.imm_i() == 0xFFFFFFFFU);
  // lb x15, 0x555(x0)
  STATIC_REQUIRE(Instruction{0x55500783}.imm_i() == 0x555U);
  // lhu x18, 0xAAA(x22)
  STATIC_REQUIRE(Instruction{0xaaab5903}.imm_i() == 0xFFFFFAAAU);
  // addi x8, x18, 0xE75
  STATIC_REQUIRE(Instruction{0xe7590413}.imm_i() == 0xFFFFFE75U);
  // sltiu x21, x17, 0x1B1
  STATIC_REQUIRE(Instruction{0x1b18ba93}.imm_i() == 0x1B1U);
  // ori x12, x5, 0xF29
  STATIC_REQUIRE(Instruction{0xf292e613}.imm_i() == 0xFFFFFF29U);
  // andi x10, x1, 0x563
  STATIC_REQUIRE(Instruction{0x5630f513}.imm_i() == 0x563U);
  // ecall
  STATIC_REQUIRE(Instruction{0x00000073}.imm_i() == 0x000U);
  // ebreak
  STATIC_REQUIRE(Instruction{0x00100073}.imm_i() == 0x001U);
  // mret
  STATIC_REQUIRE(Instruction{0x30200073}.imm_i() == 0x302U);
}

TEST_CASE("Immediates of S-type instructions are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the immediates of some S-type instructions
  // sw x17, 0x000(x9)
  STATIC_REQUIRE(Instruction{0x0114a023}.imm_s() == 0x000U);
  // sb x15, 0xFFF(x0)
  STATIC_REQUIRE(Instruction{0xfef00fa3}.imm_s() == 0xFFFFFFFFU);
  // sh x18, 0xAAA(x22)
  STATIC_REQUIRE(Instruction{0xab2b1523}.imm_s() == 0xFFFFFAAAU);
}

TEST_CASE("Immediates of B-type instructions are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the immediates of some B-type instructions
  // beq x28, x31, 0x000
  STATIC_REQUIRE(Instruction{0x01fe0063}.imm_b() == 0x000U);
  // bne x17, x9, 0x1FFE
  STATIC_REQUIRE(Instruction{0xfe989fe3}.imm_b() == 0xFFFFFFFEU);
  // blt x15, x0, 0x554
  STATIC_REQUIRE(Instruction{0x5407ca63}.imm_b() == 0x554U);
  // bge x18, x22, 0xAAA
  STATIC_REQUIRE(Instruction{0x2b6955e3}.imm_b() == 0xAAAU);
  // bltu x8, x18, 0x1E76
  STATIC_REQUIRE(Instruction{0xe7246be3}.imm_b() == 0xFFFFFE76U);
  // bgeu x21, x17, 0x1B2
  STATIC_REQUIRE(Instruction{0x1b1af963}.imm_b() == 0x1B2U);
}

TEST_CASE("Immediates of U-type instructions are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the immediates of some U-type instructions
  // lui x5, 0x00000
  STATIC_REQUIRE(Instruction{0x000002b7}.imm_u() == 0x00000000U);
  // auipc x5, 0xFFFFF
  STATIC_REQUIRE(Instruction{0xfffff297}.imm_u() == 0xFFFFF000U);
  // lui x5, 0x55555
  STATIC_REQUIRE(Instruction{0x555552b7}.imm_u() == 0x55555000U);
  // auipc x5, 0xAAAAA
  STATIC_REQUIRE(Instruction{0xaaaaa297}.imm_u() == 0xAAAAA000U);
  // lui x5, 0xBEFF9
  STATIC_REQUIRE(Instruction{0xbeff92b7}.imm_u() == 0xBEFF9000U);
  // auipc x5, 0x0D4FA
  STATIC_REQUIRE(Instruction{0x0d4fa297}.imm_u() == 0x0D4FA000U);
  // lui x5, 0x554A7
  STATIC_REQUIRE(Instruction{0x554a72b7}.imm_u() == 0x554A7000U);
}

TEST_CASE("Immediates of J-type instructions are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the immediates of some J-type instructions
  // jal x28, 0x0
  STATIC_REQUIRE(Instruction{0x00000e6f}.imm_j() == 0x0U);
  // jal x5, 0x1FFFFE
  STATIC_REQUIRE(Instruction{0xfffff2ef}.imm_j() == 0xFFFFFFFEU);
  // jal x17, 0xAAAAA
  STATIC_REQUIRE(Instruction{0x2abaa8ef}.imm_j() == 0xAAAAAU);
  // jal x18, 0xD6354
  STATIC_REQUIRE(Instruction{0x354d696f}.imm_j() == 0xD6354U);
  // jal x8, 0x1B67D0
  STATIC_REQUIRE(Instruction{0xfd0b646f}.imm_j() == 0xFFFB67D0U);
  // jal x21, 0xF955A
  STATIC_REQUIRE(Instruction{0x55af9aef}.imm_j() == 0xF955AU);
}

#include "emulator/instruction.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Instructions correctly store their value",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the values of some arbitrary instructions
  // lui x1, 0x561A9
  STATIC_REQUIRE(Instruction{0x561a90b7}.word() == 0x561a90b7);
  // auipc x20, 0x93E78
  STATIC_REQUIRE(Instruction{0x93e78a17}.word() == 0x93e78a17);
  // bne x18, x0, start
  STATIC_REQUIRE(Instruction{0xfe091ce3}.word() == 0xfe091ce3);
  // lh x24, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x68919c03}.word() == 0x68919c03);
  // andi x5, x31, 0x78F
  STATIC_REQUIRE(Instruction{0x78fff293}.word() == 0x78fff293);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.word() == 0x40000033);
  // csrrwi x9, 0x50A, 0x5
  STATIC_REQUIRE(Instruction{0x50a2d4f3}.word() == 0x50a2d4f3);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.word() == 0x03e53733);
}

TEST_CASE("Instruction opcodes are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the opcodes of some arbitrary instructions
  // lui x1, 0x561A9
  STATIC_REQUIRE(Instruction{0x561a90b7}.opcode() == 0b0110111);
  // auipc x20, 0x93E78
  STATIC_REQUIRE(Instruction{0x93e78a17}.opcode() == 0b0010111);
  // bne x18, x0, start
  STATIC_REQUIRE(Instruction{0xfe091ce3}.opcode() == 0b1100011);
  // lh x24, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x68919c03}.opcode() == 0b0000011);
  // andi x5, x31, 0x78F
  STATIC_REQUIRE(Instruction{0x78fff293}.opcode() == 0b0010011);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.opcode() == 0b0110011);
  // csrrwi x9, 0x50A, 0x5
  STATIC_REQUIRE(Instruction{0x50a2d4f3}.opcode() == 0b1110011);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.opcode() == 0b0110011);
}

TEST_CASE("Instruction funct3 fields are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the funct3 fields of some instructions
  // bne x18, x0, start
  STATIC_REQUIRE(Instruction{0xfe091ce3}.funct3() == 0b001);
  // lbu x24, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x6891cc03}.funct3() == 0b100);
  // sw x23, 0x76C(x8)
  STATIC_REQUIRE(Instruction{0x77742623}.funct3() == 0b010);
  // andi x5, x31, 0x78F
  STATIC_REQUIRE(Instruction{0x78fff293}.funct3() == 0b111);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.funct3() == 0b000);
  // csrrwi x9, 0x50A, 0x5
  STATIC_REQUIRE(Instruction{0x50a2d4f3}.funct3() == 0b101);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.funct3() == 0b011);
}

TEST_CASE("Instruction funct7 fields are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the funct7 fields of some instructions
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.funct7() == 0b0100000);
  // xor x11, x3, x17
  STATIC_REQUIRE(Instruction{0x0111c5b3}.funct7() == 0b0000000);
  // sltu x24, x16, x28
  STATIC_REQUIRE(Instruction{0x01c83c33}.funct7() == 0b0000000);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.funct7() == 0b0000001);
  // rem x9, x22, x1
  STATIC_REQUIRE(Instruction{0x021b64b3}.funct7() == 0b0000001);

  // The funct7 field should also correspond to the top bits of a immediate
  // shift instruction, altough they are actually an I-type instruction
  // slli x9, x26, 21
  STATIC_REQUIRE(Instruction{0x015d1493}.funct7() == 0b0000000);
  // srai x8, x4, 15
  STATIC_REQUIRE(Instruction{0x40f25413}.funct7() == 0b0100000);
}

TEST_CASE("Instruction destination registers are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the destination registers of some instructions
  // lui x1, 0x561A9
  STATIC_REQUIRE(Instruction{0x561a90b7}.rd() == 0b00001);
  // auipc x20, 0x93E78
  STATIC_REQUIRE(Instruction{0x93e78a17}.rd() == 0b10100);
  // jal x31, 0x14EF62
  STATIC_REQUIRE(Instruction{0xf634efef}.rd() == 0b11111);
  // lh x24, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x68919c03}.rd() == 0b11000);
  // andi x5, x31, 0x78F
  STATIC_REQUIRE(Instruction{0x78fff293}.rd() == 0b00101);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.rd() == 0b00000);
  // csrrwi x9, 0x50A, 0x5
  STATIC_REQUIRE(Instruction{0x50a2d4f3}.rd() == 0b01001);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.rd() == 0b01110);
}

TEST_CASE("Instruction first source registers are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the first source registers of some instructions
  // bne x18, x0, start
  STATIC_REQUIRE(Instruction{0xfe091ce3}.rs1() == 0b10010);
  // lbu x24, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x6891cc03}.rs1() == 0b00011);
  // sw x23, 0x76C(x8)
  STATIC_REQUIRE(Instruction{0x77742623}.rs1() == 0b01000);
  // andi x5, x31, 0x78F
  STATIC_REQUIRE(Instruction{0x78fff293}.rs1() == 0b11111);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.rs1() == 0b00000);
  // csrrwi x9, 0x50A, 0x5
  STATIC_REQUIRE(Instruction{0x50a2d4f3}.rs1() == 0b00101);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.rs1() == 0b01010);
}

TEST_CASE("Instruction second source registers are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the second source registers of some instructions
  // bne x18, x0, start
  STATIC_REQUIRE(Instruction{0xfe091ce3}.rs2() == 0b00000);
  // bgeu x2, x25, 0x450
  STATIC_REQUIRE(Instruction{0x45917863}.rs2() == 0b11001);
  // sw x23, 0x76C(x8)
  STATIC_REQUIRE(Instruction{0x77742623}.rs2() == 0b10111);
  // sb x31, 0x689(x3)
  STATIC_REQUIRE(Instruction{0x69f184a3}.rs2() == 0b11111);
  // sub x0, x0, x0
  STATIC_REQUIRE(Instruction{0x40000033}.rs2() == 0b00000);
  // mulhu x14, x10, x30
  STATIC_REQUIRE(Instruction{0x03e53733}.rs2() == 0b11110);
}

TEST_CASE("Immediates of I-type instructions are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the immediates of some I-type instructions
  // jalr x28, 0x000(x31)
  STATIC_REQUIRE(Instruction{0x000f8e67}.imm_i() == 0x000);
  // lw x17, 0xFFF(x9)
  STATIC_REQUIRE(Instruction{0xfff4a883}.imm_i() == 0xFFFFFFFF);
  // lb x15, 0x555(x0)
  STATIC_REQUIRE(Instruction{0x55500783}.imm_i() == 0x555);
  // lhu x18, 0xAAA(x22)
  STATIC_REQUIRE(Instruction{0xaaab5903}.imm_i() == 0xFFFFFAAA);
  // addi x8, x18, 0xE75
  STATIC_REQUIRE(Instruction{0xe7590413}.imm_i() == 0xFFFFFE75);
  // sltiu x21, x17, 0x1B1
  STATIC_REQUIRE(Instruction{0x1b18ba93}.imm_i() == 0x1B1);
  // ori x12, x5, 0xF29
  STATIC_REQUIRE(Instruction{0xf292e613}.imm_i() == 0xFFFFFF29);
  // andi x10, x1, 0x563
  STATIC_REQUIRE(Instruction{0x5630f513}.imm_i() == 0x563);
  // ecall
  STATIC_REQUIRE(Instruction{0x00000073}.imm_i() == 0x000);
  // ebreak
  STATIC_REQUIRE(Instruction{0x00100073}.imm_i() == 0x001);
  // mret
  STATIC_REQUIRE(Instruction{0x30200073}.imm_i() == 0x302);
}

TEST_CASE("Immediates of S-type instructions are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the immediates of some S-type instructions
  // sw x17, 0x000(x9)
  STATIC_REQUIRE(Instruction{0x0114a023}.imm_s() == 0x000);
  // sb x15, 0xFFF(x0)
  STATIC_REQUIRE(Instruction{0xfef00fa3}.imm_s() == 0xFFFFFFFF);
  // sh x18, 0xAAA(x22)
  STATIC_REQUIRE(Instruction{0xab2b1523}.imm_s() == 0xFFFFFAAA);
}

TEST_CASE("Immediates of B-type instructions are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the immediates of some B-type instructions
  // beq x28, x31, 0x000
  STATIC_REQUIRE(Instruction{0x01fe0063}.imm_b() == 0x000);
  // bne x17, x9, 0x1FFE
  STATIC_REQUIRE(Instruction{0xfe989fe3}.imm_b() == 0xFFFFFFFE);
  // blt x15, x0, 0x554
  STATIC_REQUIRE(Instruction{0x5407ca63}.imm_b() == 0x554);
  // bge x18, x22, 0xAAA
  STATIC_REQUIRE(Instruction{0x2b6955e3}.imm_b() == 0xAAA);
  // bltu x8, x18, 0x1E76
  STATIC_REQUIRE(Instruction{0xe7246be3}.imm_b() == 0xFFFFFE76);
  // bgeu x21, x17, 0x1B2
  STATIC_REQUIRE(Instruction{0x1b1af963}.imm_b() == 0x1B2);
}

TEST_CASE("Immediates of U-type instructions are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the immediates of some U-type instructions
  // lui x5, 0x00000
  STATIC_REQUIRE(Instruction{0x000002b7}.imm_u() == 0x00000000);
  // auipc x5, 0xFFFFF
  STATIC_REQUIRE(Instruction{0xfffff297}.imm_u() == 0xFFFFF000);
  // lui x5, 0x55555
  STATIC_REQUIRE(Instruction{0x555552b7}.imm_u() == 0x55555000);
  // auipc x5, 0xAAAAA
  STATIC_REQUIRE(Instruction{0xaaaaa297}.imm_u() == 0xAAAAA000);
  // lui x5, 0xBEFF9
  STATIC_REQUIRE(Instruction{0xbeff92b7}.imm_u() == 0xBEFF9000);
  // auipc x5, 0x0D4FA
  STATIC_REQUIRE(Instruction{0x0d4fa297}.imm_u() == 0x0D4FA000);
  // lui x5, 0x554A7
  STATIC_REQUIRE(Instruction{0x554a72b7}.imm_u() == 0x554A7000);
}

TEST_CASE("Immediates of J-type instructions are correctly decoded",
          "[emulator][instruction]") {
  using Instruction = selsiro_emulator::Instruction;

  // Check the immediates of some J-type instructions
  // jal x28, 0x0
  STATIC_REQUIRE(Instruction{0x00000e6f}.imm_j() == 0x0);
  // jal x5, 0x1FFFFE
  STATIC_REQUIRE(Instruction{0xfffff2ef}.imm_j() == 0xFFFFFFFE);
  // jal x17, 0xAAAAA
  STATIC_REQUIRE(Instruction{0x2abaa8ef}.imm_j() == 0xAAAAA);
  // jal x18, 0xD6354
  STATIC_REQUIRE(Instruction{0x354d696f}.imm_j() == 0xD6354);
  // jal x8, 0x1B67D0
  STATIC_REQUIRE(Instruction{0xfd0b646f}.imm_j() == 0xFFFB67D0);
  // jal x21, 0xF955A
  STATIC_REQUIRE(Instruction{0x55af9aef}.imm_j() == 0xF955A);
}

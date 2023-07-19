#ifndef EMULATOR_REGISTERS_HPP
#define EMULATOR_REGISTERS_HPP

#include "emulator/util.hpp"

#include <array>
#include <optional>

namespace selsiro_emulator {

class Registers {

public:
  [[nodiscard]] constexpr std::optional<Word>
  read(unsigned int index) const noexcept {
    if (index >= m_registers.size()) {
      return std::nullopt;
    }

    return m_registers.at(index);
  }

  constexpr void write(unsigned int index, Word data) noexcept {
    if (index >= m_registers.size() || index == 0) {
      return;
    }

    m_registers.at(index) = data;
  }

private:
  std::array<Word, 32> m_registers = {0};
};

} // namespace selsiro_emulator

#endif

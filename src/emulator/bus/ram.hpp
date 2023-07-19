#ifndef EMULATOR_BUS_RAM_HPP
#define EMULATOR_BUS_RAM_HPP

#include "bus_faults.hpp"
#include "emulator/util.hpp"

#include <tl/expected.hpp>

namespace selsiro_emulator {
template <Address StartAddr, Size NumBytes>
  requires((NumBytes % sizeof(Word) == 0) && (StartAddr % sizeof(Word) == 0))
class RAM {

public:
  // TODO: transform to C++20 ranges once all compilers used fully support it
  template <std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
  constexpr void fill(Iterator first, Sentinel last) noexcept {
    // This can be a lot simpler using std::ranges::copy and std::views::take
    // but those are not fully implemented correctly in all of the compilers in
    // use.
    auto in_iterator = first;
    auto out_iterator = m_data.begin();

    while (in_iterator != last && out_iterator != m_data.end()) {
      *out_iterator = *in_iterator;
      in_iterator++;
      out_iterator++;
    }
  }

  [[nodiscard]] constexpr tl::expected<Word, BusFault>
  read_word(Address address) const noexcept {
    if (address < StartAddr || address >= StartAddr + NumBytes) {
      return tl::unexpected(BusFault::AccessFault);
    }

    if (address % sizeof(Word) != 0) {
      return tl::unexpected(BusFault::AddressMisaligned);
    }

    return m_data.at((address - StartAddr) / sizeof(Word));
  }

private:
  std::array<Word, (NumBytes / sizeof(Word))> m_data;
};

} // namespace selsiro_emulator

#endif

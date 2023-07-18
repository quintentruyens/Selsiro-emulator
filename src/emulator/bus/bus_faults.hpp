#ifndef EMULATOR_BUS_BUS_FAULTS_HPP
#define EMULATOR_BUS_BUS_FAULTS_HPP

namespace selsiro_emulator {

enum class BusFault {
  AccessFault,
  AddressMisaligned,
};

} // namespace selsiro_emulator

#endif

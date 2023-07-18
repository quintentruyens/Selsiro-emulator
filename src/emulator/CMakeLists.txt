add_library(emulator INTERFACE)
add_library(Selsiro_emulator::emulator ALIAS emulator)

target_include_directories(emulator ${WARNING_GUARD} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                               $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
                                                               $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>)

target_link_system_libraries(
  emulator
  INTERFACE
          tl::expected)

target_compile_features(emulator INTERFACE cxx_std_20)

set_target_properties(
  emulator
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

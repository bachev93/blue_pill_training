#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

enum class OPERATING_MODE_TYPE {LOW, MIDDLE, HIGH, MODE_COUNT};
struct OperatingModeParams {
  OPERATING_MODE_TYPE mode;
  uint8_t low_threshold;
  uint8_t high_threshold;
};

namespace constants {
// battery voltage max value
const float vbat = 3.3;

const OperatingModeParams low_mode = {OPERATING_MODE_TYPE::LOW, 38, 40};
const OperatingModeParams middle_mode = {OPERATING_MODE_TYPE::MIDDLE, 40, 45};
const OperatingModeParams high_mode = {OPERATING_MODE_TYPE::HIGH, 45, 50};
}

#endif // CONSTANTS_H
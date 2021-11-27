#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

namespace constants {
// battery voltage max value
const float vbat = 3.3;

enum class OPERATING_MODE {LOW, MIDDLE, HIGH};
struct OperatingModeParams {
    OPERATING_MODE mode;
    uint8_t low_threshold;
    uint8_t high_threshold;
};
const OperatingModeParams low_mode = {OPERATING_MODE::LOW, 38, 40};
const OperatingModeParams middle_mode = {OPERATING_MODE::MIDDLE, 40, 45};
const OperatingModeParams high_mode = {OPERATING_MODE::HIGH, 45, 50};
}

#endif // CONSTANTS_H
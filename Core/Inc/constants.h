#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

#include "stm32f1xx_hal_gpio.h"

namespace thermoregulator {
enum class OperatingModeType {LOW, MIDDLE, HIGH, MODE_COUNT};
struct OperatingModeParams {
  OperatingModeType mode;
  uint8_t low_threshold;
  uint8_t high_threshold;
};

struct Led {
GPIO_TypeDef* port;
uint16_t pin;
};

namespace constants {
// battery voltage max value
const float vbat = 3.3;

const OperatingModeParams low_mode = {OperatingModeType::LOW, 38, 40};
const OperatingModeParams middle_mode = {OperatingModeType::MIDDLE, 40, 45};
const OperatingModeParams high_mode = {OperatingModeType::HIGH, 45, 50};

const Led mode_led1 = {GPIOB, GPIO_PIN_12};
const Led mode_led2 = {GPIOB, GPIO_PIN_13};
const Led mode_led3 = {GPIOB, GPIO_PIN_14};
}
}

#endif // CONSTANTS_H
#include <cstdio>
#include <numeric>

#include "auxiliary.h"

namespace thermoregulator {
OperatingMode::OperatingMode() :
  params_(constants::low_mode) {
  HAL_GPIO_WritePin(constants::mode_led1.port, constants::mode_led1.pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(constants::mode_led2.port, constants::mode_led2.pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(constants::mode_led3.port, constants::mode_led3.pin, GPIO_PIN_RESET);
}

void OperatingMode::change_mode() {
  auto cur_mode = static_cast<int>(params_.mode);
  auto mode_count = static_cast<int>(OperatingModeType::MODE_COUNT);
  OperatingModeType next_mode_type = static_cast<OperatingModeType>((cur_mode + 1) % mode_count);

  switch (next_mode_type)
  {
  case OperatingModeType::LOW:
    params_ = constants::low_mode;
    HAL_GPIO_WritePin(constants::mode_led1.port, constants::mode_led1.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(constants::mode_led2.port, constants::mode_led2.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(constants::mode_led3.port, constants::mode_led3.pin, GPIO_PIN_RESET);
    printf("low mode, yellow address LED\r\n");
  break;
  case OperatingModeType::MIDDLE:
    params_ = constants::middle_mode;
    HAL_GPIO_WritePin(constants::mode_led1.port, constants::mode_led1.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(constants::mode_led2.port, constants::mode_led2.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(constants::mode_led3.port, constants::mode_led3.pin, GPIO_PIN_RESET);
    printf("middle mode, orange address LED\r\n");
  break;
  case OperatingModeType::HIGH:
    params_ = constants::high_mode;
    HAL_GPIO_WritePin(constants::mode_led1.port, constants::mode_led1.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(constants::mode_led2.port, constants::mode_led2.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(constants::mode_led3.port, constants::mode_led3.pin, GPIO_PIN_SET);
    printf("high mode, red address LED\r\n");
  break;
  default:
    printf("unknown operating mode type\r\n");
  break;
  }
}

OperatingModeParams OperatingMode::current_mode() const {
  return params_;
}

ButtonPressType check_button_press(GPIO_TypeDef* port, uint16_t pin, uint32_t time_ms_short, uint32_t time_ms_long) {
  auto result = ButtonPressType::NO_PRESS;
  auto curr_time = HAL_GetTick();
  auto diff_time = time_ms_long - time_ms_short;

  if(HAL_GPIO_ReadPin(port, pin)) {
    result = ButtonPressType::SHORT_PRESS;
    // debounce time
    HAL_Delay(time_ms_short);

    while(HAL_GPIO_ReadPin(port, pin)) {
      if(HAL_GetTick() - curr_time >= diff_time) {
        result = ButtonPressType::LONG_PRESS;
        break;
      }
    }
  }
  // short delay to counter debounce on release
  HAL_Delay(100);
  return result;
}

DeviceStatus device_status() {
  bool state1 = HAL_GPIO_ReadPin(constants::charge_state_pin1.port, constants::charge_state_pin1.pin);
  bool state2 = HAL_GPIO_ReadPin(constants::charge_state_pin2.port, constants::charge_state_pin2.pin);

  DeviceStatus res;
  if(state1 && state2) {
    res = DeviceStatus::DEVICE_WORKING;
  } else if(!state1 && state2) {
    res = DeviceStatus::DEVICE_CHARGING;
  } else if(state1 && !state2) {
    res = DeviceStatus::DEVICE_CHARGED;
  } else {
    res = DeviceStatus::UNKNOWN;
  }

  return res;
}

void change_addr_led_behaviour(DeviceStatus dev_state) {
  switch (dev_state) {
  case DeviceStatus::DEVICE_WORKING:
    printf("device is working, address LED color depends on battery charging level\r\n");
    break;
  case DeviceStatus::DEVICE_CHARGING:
    printf("device is charging, PWM blue address LED\r\n");
    break;
  case DeviceStatus::DEVICE_CHARGED:
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    printf("device is charged, blue address LED\r\n");
    break;
  default:
    printf("unknown charging status\r\n");
    break;
  }
}

float get_battery_voltage(ADC_HandleTypeDef* hadc, int samples_size) {
  uint32_t samples[samples_size];

  HAL_ADC_Start(hadc);
  for(auto i = 0; i < samples_size; ++i) {
    HAL_ADC_PollForConversion(hadc, 1);
    samples[i] = HAL_ADC_GetValue(hadc);
  }
  HAL_ADC_Stop(hadc);

  auto average = std::accumulate(samples, samples + samples_size, 0) / samples_size;
  // TODO: change 4095 to constant 12 bit integer max val
  return constants::vbat / 4095 * average;
}
}
#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <cstdint>

#include "stm32f1xx.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_adc.h"

#include "constants.h"

namespace thermoregulator {
class OperatingMode {
  public:
    OperatingMode();
    void change_mode();
    OperatingModeParams current_mode() const;
  private:
    OperatingModeParams params_;
};

enum class ButtonPressType {SHORT_PRESS, LONG_PRESS, NO_PRESS};
ButtonPressType check_button_press(GPIO_TypeDef* port, uint16_t pin, uint32_t time_ms_short, uint32_t time_ms_long);

enum class ChargingStatus {DEVICE_CHARGING, DEVICE_CHARGED, DEVICE_WORKING, UNKNOWN};
ChargingStatus charging_status();

float get_battery_voltage(ADC_HandleTypeDef* hadc, int samples_size = 10);
}
#endif // AUXILIRY_H
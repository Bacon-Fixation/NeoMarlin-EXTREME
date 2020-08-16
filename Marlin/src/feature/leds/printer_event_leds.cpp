/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * printer_event_leds.cpp - LED color changing based on printer status
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(PRINTER_EVENT_LEDS)

#include "printer_event_leds.h"

PrinterEventLEDs printerEventLEDs;

#if HAS_LEDS_OFF_FLAG
  bool PrinterEventLEDs::leds_off_after_print; // = false
#endif

#if HAS_TEMP_HOTEND || HAS_HEATED_BED

  uint8_t PrinterEventLEDs::old_intensity = 0;

  inline uint8_t pel_intensity(const float &start, const float &current, const float &target) {
    return (uint8_t)map(constrain(current, start, target), start, target, 0.f, 255.f);
  }

  inline void pel_set_rgb(const uint8_t r, const uint8_t g, const uint8_t b) {
    leds.set_color(
      MakeLEDColor(r, g, b, 0, neo.brightness())
      #if ENABLED(NEOPIXEL_IS_SEQUENTIAL)
        , true
      #endif
    );
  }

#endif

#if HAS_TEMP_HOTEND

  void PrinterEventLEDs::onHotendHeating(const float &start, const float &current, const float &target) {
    const uint8_t blue = pel_intensity(start, current, target);
    if (blue != old_intensity) {
      old_intensity = blue;
      pel_set_rgb(255, 0, 255 - blue);
    }
  }

#endif

#if HAS_HEATED_BED

  void PrinterEventLEDs::onBedHeating(const float &start, const float &current, const float &target) {
    const uint8_t red = pel_intensity(start, current, target);
    if (red != old_intensity) {
      old_intensity = red;
      pel_set_rgb(red, 0, 255);
    }
  }
#endif
/*#if NEOPIXELX2


#if ENABLED(LED2_COLOR_PRESETS)
  const LEDColor2 LEDLights2::defaultLEDColor = MakeLEDColor(
    LED2_USER_PRESET_RED,
    LED2_USER_PRESET_GREEN,
    LED2_USER_PRESET_BLUE,
    LED2_USER_PRESET_WHITE,
    LED2_USER_PRESET_BRIGHTNESS
  );
#endif


LEDLights2 leds2;

void LEDLights2::setup() {
  #if EITHER(RGB_LED, RGBW_LED)
    if (PWM_PIN(RGB_LED_R_PIN)) SET_PWM(RGB_LED_R_PIN); else SET_OUTPUT(RGB_LED_R_PIN);
    if (PWM_PIN(RGB_LED_G_PIN)) SET_PWM(RGB_LED_G_PIN); else SET_OUTPUT(RGB_LED_G_PIN);
    if (PWM_PIN(RGB_LED_B_PIN)) SET_PWM(RGB_LED_B_PIN); else SET_OUTPUT(RGB_LED_B_PIN);
    #if ENABLED(RGBW_LED)
      if (PWM_PIN(RGB_LED_W_PIN)) SET_PWM(RGB_LED_W_PIN); else SET_OUTPUT(RGB_LED_W_PIN);
    #endif
  #endif
  TERN_(NEOPIXEL2, neo2.init());
  TERN_(PCA9533, PCA9533_init());
  TERN_(LED2_USER_PRESET_STARTUP, set_default());
}





#endif // NeoPixel End
*/
#endif // PRINTER_EVENT_LEDS

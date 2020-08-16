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
 * leds.cpp - Marlin RGB LED general support
 */

#include "../../inc/MarlinConfig.h"

#if HAS_COLOR_LEDS

#include "leds.h"

#if ENABLED(BLINKM)
  #include "blinkm.h"
#endif

#if ENABLED(PCA9632)
  #include "pca9632.h"
#endif

#if ENABLED(PCA9533)
  #include "pca9533.h"
#endif

#if ENABLED(LED_COLOR_PRESETS)
  const LEDColor LEDLights::defaultLEDColor = MakeLEDColor(
    LED_USER_PRESET_RED,
    LED_USER_PRESET_GREEN,
    LED_USER_PRESET_BLUE,
    LED_USER_PRESET_WHITE,
    LED_USER_PRESET_BRIGHTNESS
  );
#endif

#if EITHER(LED_CONTROL_MENU, PRINTER_EVENT_LEDS)
  LEDColor LEDLights::color;
  bool LEDLights::lights_on;
#endif

LEDLights leds;

void LEDLights::setup() {
  #if EITHER(RGB_LED, RGBW_LED)
    if (PWM_PIN(RGB_LED_R_PIN)) SET_PWM(RGB_LED_R_PIN); else SET_OUTPUT(RGB_LED_R_PIN);
    if (PWM_PIN(RGB_LED_G_PIN)) SET_PWM(RGB_LED_G_PIN); else SET_OUTPUT(RGB_LED_G_PIN);
    if (PWM_PIN(RGB_LED_B_PIN)) SET_PWM(RGB_LED_B_PIN); else SET_OUTPUT(RGB_LED_B_PIN);
    #if ENABLED(RGBW_LED)
      if (PWM_PIN(RGB_LED_W_PIN)) SET_PWM(RGB_LED_W_PIN); else SET_OUTPUT(RGB_LED_W_PIN);
    #endif
  #endif
  TERN_(NEOPIXEL_LED, neo.init());
  TERN_(PCA9533, PCA9533_init());
  TERN_(LED_USER_PRESET_STARTUP, set_default());
}

void LEDLights::set_color(const LEDColor &incol
  #if ENABLED(NEOPIXEL_LED)
    , bool isSequence/*=false*/
  #endif
) {

  #if ENABLED(NEOPIXEL_LED)

    const uint32_t neocolor = LEDColorWhite() == incol
                            ? neo.Color(NEO_WHITE)
                            : neo.Color(incol.r, incol.g, incol.b, incol.w);
    static uint16_t nextLed = 0;

    #ifdef NEOPIXEL_BKGD_LED_INDEX
      if (NEOPIXEL_BKGD_LED_INDEX == nextLed) {
        if (++nextLed >= neo.pixels()) nextLed = 0;
        return;
      }
    #endif

    neo.set_brightness(incol.i);

    if (isSequence) {
      neo.set_pixel_color(nextLed, neocolor);
      neo.show();
      if (++nextLed >= neo.pixels()) nextLed = 0;
      return;
    }

    neo.set_color(neocolor);

  #endif

  #if ENABLED(BLINKM)

    // This variant uses i2c to send the RGB components to the device.
    blinkm_set_led_color(incol);

  #endif

  #if EITHER(RGB_LED, RGBW_LED)

    // This variant uses 3-4 separate pins for the RGB(W) components.
    // If the pins can do PWM then their intensity will be set.
    #define UPDATE_RGBW(C,c) do { if (PWM_PIN(RGB_LED_##C##_PIN)) \
        analogWrite(pin_t(RGB_LED_##C##_PIN), incol.c); \
      else WRITE(RGB_LED_##C##_PIN, incol.c ? HIGH : LOW); }while(0)
    UPDATE_RGBW(R,r);
    UPDATE_RGBW(G,g);
    UPDATE_RGBW(B,b);
    #if ENABLED(RGBW_LED)
      UPDATE_RGBW(W,w);
    #endif

  #endif

  #if ENABLED(PCA9632)
    // Update I2C LED driver
    pca9632_set_led_color(incol);
  #endif

  TERN_(PCA9533, PCA9533_setColor(incol.r, incol.g, incol.b));

  #if EITHER(LED_CONTROL_MENU, PRINTER_EVENT_LEDS)
    // Don't update the color when OFF
    lights_on = !incol.is_off();
    if (lights_on) color = incol;
  #endif
}

#if ENABLED(LED_CONTROL_MENU)
  void LEDLights::toggle() { if (lights_on) set_off(); else update(); }
#endif

#ifdef LED_BACKLIGHT_TIMEOUT

  millis_t LEDLights::led_off_time; // = 0

  void LEDLights::update_timeout(const bool power_on) {
    const millis_t ms = millis();
    if (power_on)
      reset_timeout(ms);
    else if (ELAPSED(ms, led_off_time))
      set_off();
  }

#endif

#if (NEOPIXELX2)


#if ENABLED(LED2_COLOR_PRESETS)
  const LEDColor2 LEDLights2::defaultLEDColor = MakeLEDColor2(
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

void LEDLights2::set_color(const LEDColor2 &incol
  #if ENABLED(NEOPIXEL2)
    , bool isSequence2/*=false*/
  #endif
  ) {

  #if ENABLED(NEOPIXEL2)

    const uint32_t neocolor2 = LEDColorWhite2() == incol
                            ? neo2.Color(NEO_WHITE)
                            : neo2.Color(incol.r, incol.g, incol.b, incol.w);
    static uint16_t nextLed2 = 0;

       neo2.set_brightness(incol.i);

    if (isSequence2) {
      neo2.set_pixel_color(nextLed2, neocolor2);
      neo2.show();
      if (++nextLed2 >= neo2.pixels()) nextLed2 = 0;
      return;
    }

    neo2.set_color(neocolor2);}

  #endif



#endif // NeoPixel End
#endif  //HAS_LEDS
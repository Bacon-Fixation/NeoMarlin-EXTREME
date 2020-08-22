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
#pragma once

/**
 * leds.h - Marlin general RGB LED support
 */

#include "../../inc/MarlinConfigPre.h"

#include <string.h>

#if ENABLED(NEOPIXEL_LED)
  #include "neopixel.h"
#endif

// A white component can be passed
#if EITHER(RGBW_LED, NEOPIXEL_LED)
  #define HAS_WHITE_LED 1
#endif

/**
 * LEDcolor type for use with leds.set_color
 */
typedef struct LEDColor {
  uint8_t r, g, b
    #if HAS_WHITE_LED
      , w
      #if ENABLED(NEOPIXEL_LED)
        , i
      #endif
    #endif
  ;

  LEDColor() : r(255), g(255), b(255)
    #if HAS_WHITE_LED
      , w(255)
      #if ENABLED(NEOPIXEL_LED)
        , i(NEOPIXEL_BRIGHTNESS)
      #endif
    #endif
  {}

  LEDColor(uint8_t r, uint8_t g, uint8_t b
    #if HAS_WHITE_LED
      , uint8_t w=0
      #if ENABLED(NEOPIXEL_LED)
        , uint8_t i=NEOPIXEL_BRIGHTNESS
      #endif
    #endif
    ) : r(r), g(g), b(b)
    #if HAS_WHITE_LED
      , w(w)
      #if ENABLED(NEOPIXEL_LED)
        , i(i)
      #endif
    #endif
  {}

  LEDColor(const uint8_t (&rgbw)[4]) : r(rgbw[0]), g(rgbw[1]), b(rgbw[2])
    #if HAS_WHITE_LED
      , w(rgbw[3])
      #if ENABLED(NEOPIXEL_LED)
        , i(NEOPIXEL_BRIGHTNESS)
      #endif
    #endif
  {}

  LEDColor& operator=(const uint8_t (&rgbw)[4]) {
    r = rgbw[0]; g = rgbw[1]; b = rgbw[2];
    TERN_(HAS_WHITE_LED, w = rgbw[3]);
    return *this;
  }

  LEDColor& operator=(const LEDColor &right) {
    if (this != &right) memcpy(this, &right, sizeof(LEDColor));
    return *this;
  }

  bool operator==(const LEDColor &right) {
    if (this == &right) return true;
    return 0 == memcmp(this, &right, sizeof(LEDColor));
  }

  bool operator!=(const LEDColor &right) { return !operator==(right); }

  bool is_off() const {
    return 3 > r + g + b
      #if HAS_WHITE_LED
        + w
      #endif
    ;
  }
} LEDColor;

/**
 * Color helpers and presets
 */
#if HAS_WHITE_LED
  #if ENABLED(NEOPIXEL_LED)
    #define MakeLEDColor(R,G,B,W,I) LEDColor(R, G, B, W, I)
  #else
    #define MakeLEDColor(R,G,B,W,I) LEDColor(R, G, B, W)
  #endif
#else
  #define MakeLEDColor(R,G,B,W,I)   LEDColor(R, G, B)
#endif

#define LEDColorOff()             LEDColor(  0,   0,   0)
#define LEDColorRed()             LEDColor(255,   0,   0)
#if ENABLED(LED_COLORS_REDUCE_GREEN)
  #define LEDColorOrange()        LEDColor(255,  25,   0)
  #define LEDColorYellow()        LEDColor(255,  75,   0)
#else
  #define LEDColorOrange()        LEDColor(255,  80,   0)
  #define LEDColorYellow()        LEDColor(255, 255,   0)
#endif
#define LEDColorGreen()           LEDColor(  0, 255,   0)
#define LEDColorBlue()            LEDColor(  0,   0, 255)
#define LEDColorIndigo()          LEDColor(  0, 255, 255)
#define LEDColorViolet()          LEDColor(255,   0, 255)
#if HAS_WHITE_LED && DISABLED(RGB_LED)
  #define LEDColorWhite()         LEDColor(  0,   0,   0, 255)
#else
  #define LEDColorWhite()         LEDColor(255, 255, 255)
#endif

class LEDLights {
public:
  LEDLights() {} // ctor

  static void setup(); // init()

  static void set_color(const LEDColor &color
    #if ENABLED(NEOPIXEL_LED)
      , bool isSequence=false
    #endif
  );

  inline void set_color(uint8_t r, uint8_t g, uint8_t b
    #if HAS_WHITE_LED
      , uint8_t w=0
      #if ENABLED(NEOPIXEL_LED)
        , uint8_t i=NEOPIXEL_BRIGHTNESS
      #endif
    #endif
    #if ENABLED(NEOPIXEL_LED)
      , bool isSequence=false
    #endif
  ) {
    set_color(MakeLEDColor(r, g, b, w, i)
      #if ENABLED(NEOPIXEL_LED)
        , isSequence
      #endif
    );
  }

  static inline void set_off()   { set_color(LEDColorOff()); }
  static inline void set_green() { set_color(LEDColorGreen()); }
  static inline void set_white() { set_color(LEDColorWhite()); }

  #if ENABLED(LED_COLOR_PRESETS)
    static const LEDColor defaultLEDColor;
    static inline void set_default()  { set_color(defaultLEDColor); }
    static inline void set_red()      { set_color(LEDColorRed()); }
    static inline void set_orange()   { set_color(LEDColorOrange()); }
    static inline void set_yellow()   { set_color(LEDColorYellow()); }
    static inline void set_blue()     { set_color(LEDColorBlue()); }
    static inline void set_indigo()   { set_color(LEDColorIndigo()); }
    static inline void set_violet()   { set_color(LEDColorViolet()); }
  #endif

  #if ENABLED(PRINTER_EVENT_LEDS)
    static inline LEDColor get_color() { return lights_on ? color : LEDColorOff(); }
  #endif

  #if EITHER(LED_CONTROL_MENU, PRINTER_EVENT_LEDS)
    static LEDColor color; // last non-off color
    static bool lights_on; // the last set color was "on"
  #endif

  #if ENABLED(LED_CONTROL_MENU)
    static void toggle();  // swap "off" with color
    static inline void update() { set_color(color); }
  #endif

  #ifdef LED_BACKLIGHT_TIMEOUT
    private:
      static millis_t led_off_time;
    public:
      static inline void reset_timeout(const millis_t &ms) {
        led_off_time = ms + LED_BACKLIGHT_TIMEOUT;
        if (!lights_on) set_default();
      }
      static void update_timeout(const bool power_on);
  #endif
};

extern LEDLights leds;
#if NEOPIXELX2

//NEO2 setup
#if NEOPIXELX2
  #include "neopixel.h"
#endif

// A white component can be passed
#if EITHER(RGBW_LED, NEOPIXEL2)
  #define HAS_WHITE_LED 1
#endif

/**
 * LEDcolor2 type for use with leds2.set_color
 */
typedef struct LEDColor2 {
  uint8_t r, g, b
    #if HAS_WHITE_LED
      , w
      #if ENABLED(NEOPIXEL2)
        , i
      #endif
    #endif
  ;

  LEDColor2() : r(255), g(255), b(255)
    #if HAS_WHITE_LED
      , w(255)
      #if ENABLED(NEOPIXEL2)
        , i(NEOPIXEL2_BRIGHTNESS)
      #endif
    #endif
  {}

  LEDColor2(uint8_t r, uint8_t g, uint8_t b
    #if HAS_WHITE_LED
      , uint8_t w=0
      #if ENABLED(NEOPIXEL2)
        , uint8_t i=NEOPIXEL2_BRIGHTNESS
      #endif
    #endif
    ) : r(r), g(g), b(b)
    #if HAS_WHITE_LED
      , w(w)
      #if ENABLED(NEOPIXEL2)
        , i(i)
      #endif
    #endif
  {}

  LEDColor2(const uint8_t (&rgbw)[4]) : r(rgbw[0]), g(rgbw[1]), b(rgbw[2])
    #if HAS_WHITE_LED
      , w(rgbw[3])
      #if ENABLED(NEOPIXEL2)
        , i(NEOPIXEL2_BRIGHTNESS)
      #endif
    #endif
  {}

  LEDColor2& operator=(const uint8_t (&rgbw)[4]) {
    r = rgbw[0]; g = rgbw[1]; b = rgbw[2];
    TERN_(HAS_WHITE_LED, w = rgbw[3]);
    return *this;
  }

  LEDColor2& operator=(const LEDColor2 &right) {
    if (this != &right) memcpy(this, &right, sizeof(LEDColor2));
    return *this;
  }

  bool operator==(const LEDColor2 &right) {
    if (this == &right) return true;
    return 0 == memcmp(this, &right, sizeof(LEDColor2));
  }

  bool operator!=(const LEDColor2 &right) { return !operator==(right); }

  bool is_off() const {
    return 3 > r + g + b
      #if HAS_WHITE_LED
        + w
      #endif
    ;
  }
} LEDColor2;

/**
 * Color helpers and presets for second neopixel channel
 */
#if HAS_WHITE_LED
  #if ENABLED(NEOPIXEL2)
    #define MakeLEDColor2(R,G,B,W,I) LEDColor2(R, G, B, W, I)
  #else
    #define MakeLEDColor2(R,G,B,W,I) LEDColor2(R, G, B, W)
  #endif
#else
  #define MakeLEDColor2(R,G,B,W,I)   LEDColor2(R, G, B)
#endif

#define LEDColorOff2()             LEDColor2(  0,   0,   0)
#define LEDColorRed2()             LEDColor2(255,   0,   0)
#if ENABLED(LED_COLORS_REDUCE_GREEN)
  #define LEDColorOrange2()        LEDColor2(255,  25,   0)
  #define LEDColorYellow2()        LEDColor2(255,  75,   0)
#else
  #define LEDColorOrange2()        LEDColor2(255,  80,   0)
  #define LEDColorYellow2()        LEDColor2(255, 255,   0)
#endif
#define LEDColorGreen2()           LEDColor2(  0, 255,   0)
#define LEDColorBlue2()            LEDColor2(  0,   0, 255)
#define LEDColorIndigo2()          LEDColor2(  0, 255, 255)
#define LEDColorViolet2()          LEDColor2(255,   0, 255)
#if HAS_WHITE_LED && DISABLED(RGB_LED)
  #define LEDColorWhite2()         LEDColor2(  0,   0,   0, 255)
#else
  #define LEDColorWhite2()         LEDColor2(255, 255, 255)
#endif

class LEDLights2 {
public:
  LEDLights2() {} // ctor

  static void setup(); // init()

  static void set_color(const LEDColor2 &color

  );

  inline void set_color(uint8_t r, uint8_t g, uint8_t b
    #if HAS_WHITE_LED
      , uint8_t w=0
      #if ENABLED(NEOPIXEL2)
        , uint8_t i=NEOPIXEL2_BRIGHTNESS
      #endif
    #endif

  ) {
    set_color(MakeLEDColor2(r, g, b, w, i)

    );
  }

  static inline void set_off()   { set_color(LEDColorOff2()); }
  static inline void set_green() { set_color(LEDColorGreen2()); }
  static inline void set_white() { set_color(LEDColorWhite2()); }

  #if ENABLED(NEO2_COLOR_PRESETS)
    static const LEDColor2 defaultLEDColor2;
    static inline void set_default()  { set_color(defaultLEDColor2); }
    static inline void set_red()      { set_color(LEDColorRed2()); }
    static inline void set_orange()   { set_color(LEDColorOrange2()); }
    static inline void set_yellow()   { set_color(LEDColorYellow2()); }
    static inline void set_blue()     { set_color(LEDColorBlue2()); }
    static inline void set_indigo()   { set_color(LEDColorIndigo2()); }
    static inline void set_violet()   { set_color(LEDColorViolet2()); }
  #endif

 
  #if EITHER(LED_CONTROL_MENU, PRINTER_EVENT_LEDS)
    static LEDColor2 color; // last non-off color
    static bool lights_on; // the last set color was "on"
  #endif

  #if ENABLED(LED_CONTROL_MENU)
    static void toggle();  // swap "off" with color
    static inline void update() { set_color(color); }
  #endif

  
};

extern LEDLights2 leds2;
#endif  
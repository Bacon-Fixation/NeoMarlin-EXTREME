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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if BOTH(MELODY, SPEAKER)

#include "../gcode.h"
#include "../../lcd/ultralcd.h" // i2c-based BUZZ
#include "../../libs/buzzer.h"  // Buzzer, if possible
#include "../../../song.h" 

/**
* M330: Play the song.h
*/

void GcodeSuite::M330() {

  // this restructures the timeing to follow standard music beats 
  // closer to how MIDI players works 
  // change this to make the song slower or faster
  static int tempo = 144;


  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  // this calculates the duration of a whole note in ms (60s/tempo)*4 beats
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0, music_maker = BEEPER_PIN, frequency = 0 ;;;
      

    // iterate over the notes of the melody.
    // Remember, the array is twice the number of notes (notes + durations)
    for (int thisNote = 0; 
      thisNote < notes * 2; 
      thisNote = thisNote + 2) {

      // calculates the duration of each note
      divider = pgm_read_word_near(melody+thisNote + 1);
      if (divider > 0) {
        // regular note, just proceed
        noteDuration = (wholenote) / divider;
      } 
      else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
      }
       // we only play the note for 90% of the duration, leaving 10% as a pause
        frequency = pgm_read_word_near(melody+thisNote);
      tone(music_maker,frequency , noteDuration * 0.9);

      // Wait for the specief duration before playing the next note.
      delay(noteDuration);


    }
  
 // BUZZ(noteDuration, frequency);

}

  void end_loop() {
    // no need to repeat the melody.
  }


#endif // MELODY


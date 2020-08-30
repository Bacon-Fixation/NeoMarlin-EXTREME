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
#include "../../feature/melody/melody_player.h" 
#include "../../feature/melody/song.h" 

/**
* M330: Play the song.h
*/

void GcodeSuite::M330() {

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

#endif // MELODY


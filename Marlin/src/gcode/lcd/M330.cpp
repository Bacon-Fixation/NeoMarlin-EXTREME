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

#include "../../feature/melody/melody_player.h" 

/**
* M330: Play the song.h
*/

void GcodeSuite::M330() {

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone2(buzzer2, pgm_read_word_near(melody+thisNote ), noteDuration * 0.9);

    // Wait for the specified duration before playing the next note.
    delay2(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer2);


}

#endif // MELODY


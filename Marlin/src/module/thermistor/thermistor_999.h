/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

// User-defined table 2
// Dummy Thermistor table.. It will ALWAYS read a fixed value.

const short temptable_999[][2] PROGMEM = {
  {   290 * OVERSAMPLENR, 250 },
  {   352 * OVERSAMPLENR, 240 },
  {   432 * OVERSAMPLENR, 230 },
  {   528 * OVERSAMPLENR, 220 },
  {   644 * OVERSAMPLENR, 210 },
  {   756 * OVERSAMPLENR, 200 },
  {   899 * OVERSAMPLENR, 190 },
  {  1072 * OVERSAMPLENR, 180 },
  {  1244 * OVERSAMPLENR, 170 },
  {  1460 * OVERSAMPLENR, 160 },
  {  1896 * OVERSAMPLENR, 140 },
  {  2320 * OVERSAMPLENR, 120 },
  {  2750 * OVERSAMPLENR, 100 },
  {  3185 * OVERSAMPLENR,  80 },
  {  3600 * OVERSAMPLENR,  60 },
  {  3950 * OVERSAMPLENR,  40 },
  {  4095 * OVERSAMPLENR,  28 }
};

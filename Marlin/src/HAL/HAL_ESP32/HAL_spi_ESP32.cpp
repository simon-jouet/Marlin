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

#ifdef ARDUINO_ARCH_ESP32

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../../inc/MarlinConfig.h"
#include <SPI.h>

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

// static SPISettings spiConfig;

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

#if ENABLED(SOFTWARE_SPI)
  #error "Software SPI not supported for ESP32. Use hardware SPI."
#else
  // --------------------------------------------------------------------------
  // hardware SPI
  // --------------------------------------------------------------------------

  SPIClass hspi(HSPI);
  SPISettings spiConfig(100000, MSBFIRST, SPI_MODE0);  

  void spiBegin() {
    hspi.end();
    hspi.begin(SCK_PIN, MISO_PIN, MOSI_PIN);
    hspi.setFrequency(100000);
    hspi.setHwCs(true);
  }

  void spiInit(uint8_t spiRate) {

  }

  /**
   * @brief  Receives a single byte from the SPI port.
   *
   * @return Byte received
   *
   * @details
   */
  uint8_t spiRec(void) {
    hspi.beginTransaction(spiConfig);
    uint8_t returnByte = hspi.transfer(0xFF);
    hspi.endTransaction();
    return returnByte;
  }

  /**
   * @brief  Receives a number of bytes from the SPI port to a buffer
   *
   * @param  buf   Pointer to starting address of buffer to write to.
   * @param  nbyte Number of bytes to receive.
   * @return Nothing
   */
  void spiRead(uint8_t* buf, uint16_t nbyte) {
    hspi.beginTransaction(spiConfig);
    hspi.transferBytes(NULL, buf, nbyte);
    hspi.endTransaction();
  }

  /**
   * @brief  Sends a single byte on SPI port
   *
   * @param  b Byte to send
   *
   * @details
   */
  void spiSend(uint8_t b) {
    hspi.beginTransaction(spiConfig);
    hspi.transfer(b);
    hspi.endTransaction();
  }

  /**
   * @brief  Write token and then write from 512 byte buffer to SPI (for SD card)
   *
   * @param  buf   Pointer with buffer start address
   * @return Nothing
   */
  void spiSendBlock(uint8_t token, const uint8_t* buf) {
    hspi.beginTransaction(spiConfig);
    hspi.transfer(token);
    hspi.transferBytes(const_cast<uint8_t*>(buf), NULL, 512);
    hspi.endTransaction();
  }

#endif // ENABLED(SOFTWARE_SPI)

#endif

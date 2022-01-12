/*
 * This file is part of the ExpressLRS distribution (https://github.com/ExpressLRS/ExpressLRS).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#if defined(USE_OLED_SPI) || defined(USE_OLED_SPI_SMALL) || defined(USE_OLED_I2C) // This code will not be used if the hardware does not have a OLED display. Maybe a better way to blacklist it in platformio.ini?

// Default header files for Express LRS
#include "targets.h"
// OLED specific header files.
#include "OLED.h"
#include <U8g2lib.h>    // Needed for the OLED drivers, this is a arduino package. It is maintained by platformIO
#include "XBMStrings.h" // Contains all the ELRS logos and animations for the UI

#ifdef OLED_REVERSED
    #define OLED_ROTATION U8G2_R2
#else
    #define OLED_ROTATION U8G2_R0
#endif

#ifdef USE_OLED_SPI_SMALL
U8G2_SSD1306_128X32_UNIVISION_F_4W_SW_SPI u8g2(OLED_ROTATION, GPIO_PIN_OLED_SCK, GPIO_PIN_OLED_MOSI, GPIO_PIN_OLED_CS, GPIO_PIN_OLED_DC, GPIO_PIN_OLED_RST);
#endif

#ifdef USE_OLED_SPI
U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(OLED_ROTATION, GPIO_PIN_OLED_SCK, GPIO_PIN_OLED_MOSI, GPIO_PIN_OLED_CS, GPIO_PIN_OLED_DC, GPIO_PIN_OLED_RST);
#endif

#ifdef USE_OLED_I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(OLED_ROTATION, GPIO_PIN_OLED_RST, GPIO_PIN_OLED_SCK, GPIO_PIN_OLED_SDA);
#endif

#ifdef TARGET_TX_GHOST
/**
 * helper function is used to draw xbmp on the OLED.
 * x = x position of the image
 * y = y position of the image
 * size = demensions of the box size x size, this only works for square images 1:1
 * image = XBM character string
 */
#ifndef TARGET_TX_GHOST_LITE
static void helper(int x, int y, int size,  const unsigned char * image){
    u8g2.clearBuffer();
    u8g2.drawXBMP(x, y, size, size, image);
    u8g2.sendBuffer();
}
#endif

/**
 *  ghostChase will only be called for ghost TX hardware.
 */
static void ghostChase(){
    // Using i < 16 and (i*4) to get 64 total pixels. Change to i < 32 (i*2) to slow animation.
    for(int i = 0; i < 20; i++){
        u8g2.clearBuffer();
        #ifndef TARGET_TX_GHOST_LITE
            u8g2.drawXBMP((26 + i), 16, 32, 32, ghost);
            u8g2.drawXBMP((-31 + (i*4)), 16, 32, 32, elrs32);
        #else
            u8g2.drawXBMP((26 + i), 0, 32, 32, ghost);
            u8g2.drawXBMP((-31 + (i*4)), 0, 32, 32, elrs32);
        #endif
        u8g2.sendBuffer();
    }
    /**
     *  Animation for the ghost logo expanding in the center of the screen.
     *  helper function just draw's the XBM strings.
     */
    #ifndef TARGET_TX_GHOST_LITE
        helper(38,12,40,elrs40);
        helper(36,8,48,elrs48);
        helper(34,4,56,elrs56);
        helper(32,0,64,elrs64);
    #endif
}
#endif

/**
 * Displays the ExpressLRS logo
 *
 * @param values none
 * @return void
 */
void OLED::displayLogo(){
    u8g2.begin();
    u8g2.clearBuffer();
    #ifdef TARGET_TX_GHOST
        ghostChase();
    #else
        #ifdef USE_OLED_SPI_SMALL
            u8g2.drawXBM(48, 0, 32, 32, elrs32);
        #else
            u8g2.drawXBM(32, 0, 64, 64, elrs64);
        #endif
    #endif
    u8g2.sendBuffer();
}

/**
 * Displays the ExpressLRS logo
 *
 * @param values power = PowerLevels_e
 *               rate = expresslrs_RFrates_e
 *               ratio = expresslrs_tlm_ratio_e
 * @return void
 */
void OLED::updateScreen(int power, int rate, int ratio, const char *commitStr){
    u8g2.clearBuffer();

    #ifdef USE_OLED_SPI_SMALL
        u8g2.setFont(u8g2_font_courR10_tr);
        u8g2.drawStr(0,15, getRateString(rate));
        u8g2.drawStr(70,15 , getTLMRatioString(ratio));
        u8g2.drawStr(0,32, getPowerString(power));
        u8g2.drawStr(70,32, commitStr);
    #else
        u8g2.setFont(u8g2_font_courR10_tr);
        u8g2.drawStr(0,10, "ExpressLRS");
        u8g2.drawStr(0,42, getRateString(rate));
        u8g2.drawStr(70,42 , getTLMRatioString(ratio));
        u8g2.drawStr(0,57, getPowerString(power));
        u8g2.setFont(u8g2_font_courR08_tr);
        u8g2.drawStr(70,53, "TLM");
        u8g2.drawStr(0,24, "Ver: ");
        u8g2.drawStr(38,24, commitStr);
    #endif
    u8g2.sendBuffer();
}

/**
 * Returns power level string (Char array)
 *
 * @param values power = int/enum for current power level.
 * @return const char array for power level Ex: "500 mW\0"
 */
const char * OLED::getPowerString(int power){
    switch (power)
    {
    case 0: return "10 mW";
    case 1: return "25 mW";
    case 2: return "50 mW";
    case 3: return "100 mW";
    case 4: return "250 mW";
    case 5: return "500 mW";
    case 6: return "1000 mW";
    case 7: return "2000 mW";
    default: return "ERR";
    }
}

/**
 * Returns packet rate string (Char array)
 *
 * @param values rate = int/enum for current packet rate.
 * @return const char array for packet rate Ex: "500 hz\0"
 */
const char * OLED::getRateString(int rate){
    switch (rate)
    {
    case 0: return "500 Hz";
    case 1: return "250 Hz";
    case 2: return "200 Hz";
    case 3: return "150 Hz";
    case 4: return "100 Hz";
    case 5: return "50 Hz";
    case 6: return "25 Hz";
    case 7: return "4 Hz";
    default: return "ERR";
    }
}

/**
 * Returns telemetry ratio string (Char array)
 *
 * @param values ratio = int/enum for current power level.
 * @return const char array for telemetry ratio Ex: "1:128\0"
 */
const char * OLED::getTLMRatioString(int ratio){
    switch (ratio)
    {
    case 0: return "OFF";
    case 1: return "1:128";
    case 2: return "1:64";
    case 3: return "1:32";
    case 4: return "1:16";
    case 5: return "1:8";
    case 6: return "1:4";
    case 7: return "1:2";
    default: return "ERR";
    }
}

#endif

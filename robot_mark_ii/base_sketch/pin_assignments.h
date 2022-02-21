/*
 *      Please see the README.md for a full description of this program and
 *      project.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#ifndef PIN_ASSIGNMENTS_H
#define PIN_ASSIGNMENTS_H

// Pin Definitions - Defines all of the pins used
const uint8_t POLOLU_QIK_TX_PIN        =  0; // tx Pololu qik
const uint8_t POLOLU_QIK_RX_PIN        =  1; // rx Pololu qik
const uint8_t RR_EDGE_SENSOR_PIN       =  2; // rear-right edge sensor
const uint8_t POLOLU_QIK_RESET_PIN     =  3; // reset Pololu qik
const uint8_t REAR_MUX_PIN2            =  4; // rear distance sensor mux selector pin 2
const uint8_t REAR_MUX_PIN1            =  5; // rear distance sensor mux selector pin 1
const uint8_t REAR_MUX_PIN0            =  6; // rear distance sensor mux selector pin 0
const uint8_t FRONT_MUX_PIN0           =  7; // front distance sensor mux selector pin 0
const uint8_t FRONT_MUX_PIN1           =  8; // front distance sensor mux selector pin 1
const uint8_t FRONT_MUX_PIN2           =  9; // front distance sensor mux selector pin 2
const uint8_t WIFI_SPI_CS0_PIN         = 10; // SPI CS0 used by Wifi
const uint8_t WIFI_SPI_MOSI0_PIN       = 11; // SPI MOSI0 uased by Wifi
const uint8_t WIFI_SPI_MISO0_PIN       = 12; // SPI MISO0 uased by Wifi
const uint8_t MODE_BUTTON_PIN          = 13; // mode button
const uint8_t WIFI_SPI_SCK0_PIN        = 14; // SPI SCK0 used by Wifi
const uint8_t WIFI_RESET_PIN           = 15; // reset pin used by Wifi
const uint8_t WIFI_BUSY_PIN            = 16; // busy pin used by Wifi
const uint8_t RL_EDGE_SENSOR_PIN       = 17; // rear-left edge sensor
                                     //= 18; // SDA0 I2C connected to FRONT_MUX
                                     //= 19; // SCL0 I2C connected to FRONT_MUX
const uint8_t FL_SURFACE_SENSOR_PIN    = 20; // front-left surface sensor
const uint8_t FR_SURFACE_SENSOR_PIN    = 21; // front-right surface sensor
const uint8_t RR_SURFACE_SENSOR_PIN    = 22; // rear-right surface sensor
const uint8_t RL_SURFACE_SENSOR_PIN    = 23; // rear-left surface sensor
const uint8_t CTR_SURFACE_SENSOR_6_PIN = 24; // center surface sensor 6
const uint8_t CTR_SURFACE_SENSOR_5_PIN = 25; // center surface sensor 5
const uint8_t CTR_SURFACE_SENSOR_4_PIN = 26; // center surface sensor 4
const uint8_t CTR_SURFACE_SENSOR_3_PIN = 27; // center surface sensor 3
const uint8_t CTR_SURFACE_SENSOR_2_PIN = 28; // center surface sensor 2
const uint8_t CTR_SURFACE_SENSOR_1_PIN = 29; // center surface sensor 1
const uint8_t FR_EDGE_SENSOR_PIN       = 30; // front-right edge sensor
const uint8_t ENCODER_R_PHASE_B_PIN    = 31; // right encoder phase b signal
const uint8_t ENCODER_R_PHASE_A_PIN    = 32; // right encoder phase a signal
const uint8_t BUTTON_PIN               = 33; // start/stop button
const uint8_t ENCODER_L_PHASE_B_PIN    = 34; // left encoder phase b signal
const uint8_t ENCODER_L_PHASE_A_PIN    = 35; // left encoder phase a signal
const uint8_t FL_EDGE_SENSOR_PIN       = 36; // front_left edge sensor
                                     //= 37; // SCL1 I2C connected to REAR_MUX
                                     //= 38; // SDA1 I2C connected to REAR_MUX
const uint8_t PIXEL_RING_PIN           = 39; // pixel ring data pin

#endif

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
const uint8_t PIXEL_RING_PIN           =  9; // pixel ring data pin
const uint8_t FRONT_MUX_PIN0           = 10; // front distance sensor mux selector pin 0
const uint8_t FRONT_MUX_PIN1           = 11; // front distance sensor mux selector pin 1
const uint8_t FRONT_MUX_PIN2           = 12; // front distance sensor mux selector pin 2
const uint8_t LED_PIN                  = 13; // led (13 is built-in led)
const uint8_t REAR_MUX_PIN2            = 14; // rear distance sensor mux selector pin 2
const uint8_t REAR_MUX_PIN1            = 15; // rear distance sensor mux selector pin 1
const uint8_t REAR_MUX_PIN0            = 16; // rear distance sensor mux selector pin 0
const uint8_t RL_EDGE_SENSOR_PIN       = 17; // rear-left edge sensor
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

#endif

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

#ifndef ROBOT_CONSTANTS_H
#define ROBOT_CONSTANTS_H

#include <inttypes.h>

 // wheel diameter in meters (31 mm)
const double WHEEL_DIAM_M = 0.031;

// distance between wheels,center to center (83 mm)
const double WHEEL_DIST_M = 0.083;

// encoder ticks for single wheel revolution
const double TICKS_PER_ROTATION = 3000.0; // 250:1 * 12 ticks

const double METERS_PER_TICK = (PI * WHEEL_DIAM_M) / TICKS_PER_ROTATION;
const double RADIANS_PER_TICK = (2 / TICKS_PER_ROTATION);

const uint8_t NUM_EDGE_SENSORS           = 4;
const uint8_t NUM_FRONT_SURFACE_SENSORS  = 2;
const uint8_t NUM_CENTER_SURFACE_SENSORS = 6;
const uint8_t NUM_REAR_SURFACE_SENSORS   = 2;
const uint8_t NUM_FRONT_DISTANCE_SENSORS = 4;
const uint8_t NUM_REAR_DISTANCE_SENSORS  = 5;

const int NUM_VEL_SAMPLES = 10;

const uint8_t FL_EDGE = 0;
const uint8_t FR_EDGE = 1;
const uint8_t RL_EDGE = 2;
const uint8_t RR_EDGE = 3;

const uint16_t EDGE_THRESHOLD = 1000;

const double CRUISE_SPEED = 0.9;
const double TURN_SPEED = 0.4;

#endif

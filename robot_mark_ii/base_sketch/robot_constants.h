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

// These are constants based on the physical build of the robot.
// Your robot build may vary.

 // wheel diameter in meters (31 mm)
const double WHEEL_DIAM_M = 0.031;

// distance between wheels,center to center (83 mm)
const double WHEEL_DIST_M = 0.083;

// encoder ticks for single wheel revolution
const double TICKS_PER_ROTATION = 3000.0; // 250:1 * 12 ticks

// Code will use Radians (angular velocity), but meters is provided for reference.
const double METERS_PER_TICK = (PI * WHEEL_DIAM_M) / TICKS_PER_ROTATION;
const double RADIANS_PER_TICK = (2 / TICKS_PER_ROTATION);

// These constants are based on measurements of the motor performance using a
// 5v power source. Your mileage may vary and you should verify your robots
// values by running the motors at full speed (1) for a second and recording
// the encoder values.

// When motor is run at full for one second, this is approximately
// how many ticks it will turn.
const double MAX_TICKS_PER_SECOND = 3600.0;

// This is the number of wheel rotation when run at full for one second.
const double MAX_ROTATIONS_PER_SECOND = MAX_TICKS_PER_SECOND / TICKS_PER_ROTATION;

// This is the max meters per second when run at full speed. Just for reference,
// code will use radians.
const double MAX_METERS_PER_SECOND = MAX_ROTATIONS_PER_SECOND * (PI * WHEEL_DIAM_M);

// This is the max radians per second when run at full speed.
// There are 2 radians in 360 degrees.
const double MAX_RADIANS_PER_SECOND = MAX_ROTATIONS_PER_SECOND * 2;

// Constants used with PID controller in the motor controller
const double KP(0.25);
const double KI(0.0008);
const double KD(0.0005);

// Motor speeds can be between -1 (reverse) and 1 (forward). These constants
// are defined for normal cruising speed (forward or reverse), and speed
// when performing any turn.
const double CRUISE_SPEED = (0.9 * MAX_RADIANS_PER_SECOND); // 90% of max
const double TURN_SPEED = (0.4 * MAX_RADIANS_PER_SECOND); // 40% of max

const uint8_t NUM_EDGE_SENSORS           = 4;
const uint8_t NUM_FRONT_SURFACE_SENSORS  = 2;
const uint8_t NUM_CENTER_SURFACE_SENSORS = 6;
const uint8_t NUM_REAR_SURFACE_SENSORS   = 2;
const uint8_t NUM_FRONT_DISTANCE_SENSORS = 4;
const uint8_t NUM_REAR_DISTANCE_SENSORS  = 5;

const uint8_t FL_EDGE = 0;
const uint8_t FR_EDGE = 1;
const uint8_t RL_EDGE = 2;
const uint8_t RR_EDGE = 3;

// When the sensor value is over this threshold, then it is over an 'edge'.
const uint16_t EDGE_THRESHOLD = 1000;

const uint16_t NUM_PIXELS_ON_RING = 16;

#endif
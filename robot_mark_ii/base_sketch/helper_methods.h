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
 
 #ifndef HELPER_METHODS_H
 #define HELPER_METHODS_H

// Arduino includes
#include <inttypes.h>

// Local includes
#include "callback_context.h"

void initializeMotorsAndEncoders(CallbackContext* context);
void initializeDistanceSensors(CallbackContext* context);
void initializeEdgeSensors(CallbackContext* context);
void initializeSurfaceSensors(CallbackContext* context);

void resetEncodersAndVelocities(CallbackContext* context);
void readEncoders(void* context);
void readEdgeSensors(void* context);
void readSurfaceSensors(void* context);
void readDistanceSensors(void* context);
void calibrateSurfaceSensors(CallbackContext* context);

double getSampledVelocity(uint8_t motor, CallbackContext* context);
void printVelocities(void* context);

void stopMotors(CallbackContext* context);
void goForward(CallbackContext* context);
void goReverse(CallbackContext* context);
void turnForwardLeft(CallbackContext* context);
void turnReverseLeft(CallbackContext* context);
void turnForwardRight(CallbackContext* context);
void turnReverseRight(CallbackContext* context);

 #endif

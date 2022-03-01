//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//
 
 #ifndef HELPER_METHODS_H
 #define HELPER_METHODS_H

// Arduino includes
#include <inttypes.h>

// Local includes
#include "callback_context.h"

PixelRing* initializePixelRing();

void initializeMotorsAndEncoders(CallbackContext* context);
void initializeDistanceSensors(CallbackContext* context);
void initializeEdgeSensors(CallbackContext* context);
void initializeSurfaceSensors(CallbackContext* context);
void initializePixelRing(CallbackContext* context);
void resetEncoders(CallbackContext* context);
long readEncoder(Motor motor, CallbackContext* context);

void adjustMotorSpeeds(void* context);

void readEdgeSensors(void* context);
void readSurfaceSensors(void* context);
void readDistanceSensors(void* context);
void adjustPixelRing(void* context);
void calibrateSurfaceSensors(CallbackContext* context);

 #endif

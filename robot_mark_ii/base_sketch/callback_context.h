//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//
 
#ifndef CALLBACK_CONTEXT_H
#define CALLBACK_CONTEXT_H

// Arduino includes
#include <inttypes.h>

// Third party includes
#include <QTRSensors.h>

// My includes
#include <MotorManager.h>
#include <MotorController.h>
#include <VL6180I2CMux.h>

// Local includes
#include "robot_constants.h"
#include "pixel_ring.h"

// Data structure used to store state between callback calls.
struct CallbackContext {  
  // Edge sensors
  QTRSensors* edgeSensors;
  
  // Surface sensors
  QTRSensors* centerSurfaceSensors;
  QTRSensors* frontSurfaceSensors;
  QTRSensors* rearSurfaceSensors;
  
  // Distance sensors
  VL6180I2CMux* frontDistanceSensorMux;
  VL6180I2CMux* rearDistanceSensorMux;
  
  // Motor and encoder manager
  MotorManager* motorManager;

  // Motor controller
  MotorController* motorController;

  uint16_t edgeSensorValues[NUM_EDGE_SENSORS];
  uint16_t centerSurfaceSensorValues[NUM_CENTER_SURFACE_SENSORS];
  uint16_t frontSurfaceSensorValues[NUM_FRONT_SURFACE_SENSORS];
  uint16_t rearSurfaceSensorValues[NUM_REAR_SURFACE_SENSORS];

  PixelRing* pixelRing;
};

 #endif

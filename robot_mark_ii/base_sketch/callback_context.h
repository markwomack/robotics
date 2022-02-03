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

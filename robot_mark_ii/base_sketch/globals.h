//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef GLOBALS_H
#define GLOBALS_H

// Local includes
#include "DistanceSensors.h"
#include "EdgeSensors.h"
#include "MotorsAndEncoders.h"
#include "SurfaceSensors.h"
#include "Animation.h"

// All of the sensors, motors, and pixel ring
// These are located in base_sketch
extern EdgeSensors edgeSensors;
extern DistanceSensors distanceSensors;
extern SurfaceSensors surfaceSensors;
extern MotorsAndEncoders motorsAndEncoders;
extern Animation animation;

#endif

//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// My includes
#include <TaskManager.h>

// Local includes
#include "FollowLineTask.h"
#include "robot_constants.h"
#include "SurfaceSensors.h"

void FollowLineTask::setup(void) {
  _movementState = STOPPED;
  
  // set the stuff into the tasks
  _readSurfaceSensorsTask.setSurfaceSensors(_surfaceSensors);
  _adjustMotorSpeedsTask.setMotorsAndEncoders(_motorsAndEncoders);
  _adjustAnimationTask.setAnimation(_animation);
    
  // register the methods used when running this behavior
  taskManager.addTask(&_readSurfaceSensorsTask, 50);
  //taskManager.addTask(&_adjustMotorSpeedsTask, 10);
  taskManager.addTask(&_adjustAnimationTask, 50);
}

void FollowLineTask::start(void) {
  _animation->setAnimationState(GREEN_CW);
}

void FollowLineTask::update(void) {
  DebugMsgs.debug().println("Updating FollowLineTask");
  
  if (DebugMsgs.isLevelEnabled(DEBUG)) {
    uint16_t values[NUM_CENTER_SURFACE_SENSORS];
    _surfaceSensors->getSensorValues(S_FRONT, values);
    DebugMsgs.print("");
    for (int x = 0; x < NUM_FRONT_SURFACE_SENSORS; x++) {
      DebugMsgs.print(values[x]).print(" ");
    }
    DebugMsgs.println();
    _surfaceSensors->getSensorValues(S_CENTER, values);
    DebugMsgs.print("");
    for (int x = 0; x < NUM_CENTER_SURFACE_SENSORS; x++) {
      DebugMsgs.print(values[x]).print(" ");
    }
    DebugMsgs.println();
    _surfaceSensors->getSensorValues(S_REAR, values);
    DebugMsgs.print("");
    for (int x = 0; x < NUM_REAR_SURFACE_SENSORS; x++) {
      DebugMsgs.print(values[x]).print(" ");
    }
    DebugMsgs.println();
  }
}

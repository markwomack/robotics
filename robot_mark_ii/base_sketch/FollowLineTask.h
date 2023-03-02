//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef FOLLOWLINETASK_H
#define FOLLOWLINETASK_H

#include "BehaviorTask.h"
#include "AdjustMotorSpeedsTask.h"
#include "AdjustAnimationTask.h"
#include "ReadSurfaceSensorsTask.h"

enum FollowState {
  AT_START,
  FOLLOW,
  AT_STOP
};

enum LineLocation {
  LINE_IS_CENTER,
  LINE_IS_LEFT,
  LINE_IS_RIGHT,
  LINE_IS_LOST
};

class FollowLineTask : public BehaviorTask {
  public:
    FollowLineTask() {};
    void setup(void);
    void start(void);
    void update(void);
    void stop(void);

    LineLocation checkLineLocation(void);
    
  protected:
    ReadSurfaceSensorsTask _readSurfaceSensorsTask;
    AdjustMotorSpeedsTask _adjustMotorSpeedsTask;
    AdjustAnimationTask _adjustAnimationTask;
    
    uint16_t _frontValues[NUM_FRONT_SURFACE_SENSORS];
    uint16_t _centerValues[NUM_CENTER_SURFACE_SENSORS];
    uint16_t _rearValues[NUM_REAR_SURFACE_SENSORS];

    FollowState _currentState;
    LineLocation _lineLocation;
};

#endif // FOLLOWLINETASK_H

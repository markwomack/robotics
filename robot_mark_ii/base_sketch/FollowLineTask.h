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

class FollowLineTask : public BehaviorTask {
  public:
    FollowLineTask() {};

    void setup(void);
    void start(void);
    void update(void);
    
  protected:
    ReadSurfaceSensorsTask _readSurfaceSensorsTask;
    AdjustMotorSpeedsTask _adjustMotorSpeedsTask;
    AdjustAnimationTask _adjustAnimationTask;
};

#endif // FOLLOWLINETASK_H

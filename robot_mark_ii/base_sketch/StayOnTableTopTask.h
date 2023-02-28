//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef STAYONTABLETOPTASK_H
#define STAYONTABLETOPTASK_H

#include "BehaviorTask.h"
#include "ReadEdgeSensorsTask.h"
#include "AdjustMotorSpeedsTask.h"
#include "AdjustAnimationTask.h"

class StayOnTableTopTask : public BehaviorTask {

  public:
    StayOnTableTopTask() {};

    void setup(void);
    void update(void);
    void stop(void);

  private:
    ReadEdgeSensorsTask _readEdgeSensorsTask;
    AdjustMotorSpeedsTask _adjustMotorSpeedsTask;
    AdjustAnimationTask _adjustAnimationTask;
};


#endif // STAYONTABLETOPTASK_H

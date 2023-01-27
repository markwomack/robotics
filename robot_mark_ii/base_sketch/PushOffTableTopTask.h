//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef PUSHOFFTABLETOPTASK_H
#define PUSHOFFTABLETOPTASK_H

// Local includes
#include "BehaviorTask.h"
#include "ReadEdgeSensorsTask.h"
#include "AdjustMotorSpeedsTask.h"
#include "AdjustPixelRingTask.h"

enum PushObjectOffTableState {
  SEEKING,
  SPIN_SEEKING,
  LOCATED,
  ACQUIRED,
  PUSHING_OFF,
  CELEBRATE,
  DONE
};

enum FieldLocation {
  NOT_FOUND,
  TO_LEFT,
  AT_CENTER,
  TO_RIGHT,
  CAPTURED
};

class PushOffTableTopTask : public BehaviorTask {
  public:
  
    PushOffTableTopTask() {};

    void setup(void);
    void start(void);
    void update(void);
    void stop();
  
  private:
    PushObjectOffTableState _logicState;
    int _jiggerCount;
    FieldLocation _lastFrontLocation;

    ReadEdgeSensorsTask _readEdgeSensorsTask;
    AdjustMotorSpeedsTask _adjustMotorSpeedsTask;
    AdjustPixelRingTask _adjustPixelRingTask;
    
    void handleSeekingState(void);
    void handleSpinSeekingState(void);
    void handleLocatedState(void);
    void handleAcquiredState();
    void handlePushingOff();
    void handleCelebrate();
    FieldLocation adjustForJigger(FieldLocation inputFieldLocation);
    FieldLocation checkForObject(Orientation orientation);
};

#endif // PUSHOFFTABLETOPTASK_H

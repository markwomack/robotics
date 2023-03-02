//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//
 
#ifndef CALIBRATESURFACESENSORSTASK_H
#define CALIBRATESURFACESENSORSTASK_H

// My includes
#include "BehaviorTask.h"
#include "FollowLineTask.h"
#include "Animation.h"

class CalibrateSurfaceSensorsTask : public BehaviorTask {
  public:
    CalibrateSurfaceSensorsTask() {};

    void start(void) {
      DebugMsgs.debug().println("Calibrating of surface sensors...");
      _surfaceSensors->resetCalibration();
      _animation->setAnimationState(GREEN_COUNTDOWN);
      _currentMode = FirstPause;
      _count = 0;
    }
    
    void update(void) {
      switch(_currentMode) {
        case FirstPause:
        case SecondPause: {
          _animation->update();
          delay(200);
          _count++;
          if (_count == 16) {
            if (_currentMode == FirstPause) {
              _currentMode = FirstCalibrate;
            } else {
              _currentMode = SecondCalibrate;
            }
            _count = 0;
            _animation->setAnimationState(BLUE_FADE);
          }
        }
        break;
          
        case FirstCalibrate:
        case SecondCalibrate: {
          _animation->update();
          _surfaceSensors->calibrate();
          _count++;
          if (_count == 200) {
            if (_currentMode == FirstCalibrate) {
              _currentMode = SecondPause;
            _animation->setAnimationState(GREEN_COUNTDOWN);
            } else {
              _currentMode = Complete;
            }
            _count = 0;
          }
        }
        break;
          
        case Complete: {
          _animation->setAnimationState(OFF);
          delay(100);
          _animation->setAnimationState(WHITE_FADE);
          delay(100);
          _animation->setAnimationState(OFF);
          delay(100);
          _animation->setAnimationState(WHITE_FADE);
          delay(100);
          _animation->setAnimationState(OFF);
          delay(100);
          _animation->setAnimationState(WHITE_FADE);
          delay(100);
          _animation->setAnimationState(OFF);

          // print the calibration minimum values measured when emitters were on
          if (DebugMsgs.isLevelEnabled(DEBUG)) {
            DebugMsgs.println("Calibration results:");
            DebugMsgs.println("Minimums:");
            uint16_t values[NUM_CENTER_SURFACE_SENSORS];
            _surfaceSensors->getCalibrationMinValues(S_FRONT, values);
            DebugMsgs.print("");
            for (int x = 0; x < NUM_FRONT_SURFACE_SENSORS; x++) {
              DebugMsgs.print(values[x]).print(" ");
            }
            DebugMsgs.println();
            _surfaceSensors->getCalibrationMinValues(S_CENTER, values);
            DebugMsgs.print("");
            for (int x = 0; x < NUM_CENTER_SURFACE_SENSORS; x++) {
              DebugMsgs.print(values[x]).print(" ");
            }
            DebugMsgs.println();
            _surfaceSensors->getCalibrationMinValues(S_REAR, values);
            DebugMsgs.print("");
            for (int x = 0; x < NUM_REAR_SURFACE_SENSORS; x++) {
              DebugMsgs.print(values[x]).print(" ");
            }
            DebugMsgs.println();
            DebugMsgs.println("Maximums:");
            _surfaceSensors->getCalibrationMaxValues(S_FRONT, values);
            DebugMsgs.print("");
            for (int x = 0; x < NUM_FRONT_SURFACE_SENSORS; x++) {
              DebugMsgs.print(values[x]).print(" ");
            }
            DebugMsgs.println();
            _surfaceSensors->getCalibrationMaxValues(S_CENTER, values);
            DebugMsgs.print("");
            for (int x = 0; x < NUM_CENTER_SURFACE_SENSORS; x++) {
              DebugMsgs.print(values[x]).print(" ");
            }
            DebugMsgs.println();
            _surfaceSensors->getCalibrationMaxValues(S_REAR, values);
            DebugMsgs.print("");
            for (int x = 0; x < NUM_REAR_SURFACE_SENSORS; x++) {
              DebugMsgs.print(values[x]).print(" ");
            }
            DebugMsgs.println();
          }

          DebugMsgs.debug().println("Calibration complete");
          taskManager.stop();
          taskManager.removeTask(_taskToken);
          FollowLineTask* followLineTask = new FollowLineTask();

          taskManager.addTask(followLineTask, 500);
          DebugMsgs.debug().println("Ready to follow lines!");
        }
        break;
          
        default:
          break;
      }
    };
    
  protected:
    enum Mode {
      FirstPause,
      FirstCalibrate,
      SecondPause,
      SecondCalibrate,
      Complete
    };
    Mode _currentMode;
    uint16_t _count;
  
};

#endif // CALIBRATESURFACESENSORSTASK_H

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
      _animation->setAnimationState(GREEN_COUNTDOWN);
      _currentMode = FirstPause;
      _count = 0;
    }
    
    void update(void) {
      switch(_currentMode) {
        case FirstPause:
        case SecondPause: {
          _animation->update();
          delay(500);
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
          taskManager.stop();
          taskManager.removeTask(_taskToken);
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
          FollowLineTask* followLineTask = new FollowLineTask();
          taskManager.addTask(followLineTask, 10);
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

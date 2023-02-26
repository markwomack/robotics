//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//
 
#ifndef CALIBRATESURFACESENSORSTASK_H
#define CALIBRATESURFACESENSORSTASK_H

// My includes
#include "BehaviorTask.h"
#include "PixelRing.h"

class CalibrateSurfaceSensorsTask : public BehaviorTask {
  public:
    CalibrateSurfaceSensorsTask() {};

    void start(void) {
      _pixelRing->changeState(RING_GREEN_COUNTDOWN);
      _currentMode = FirstPause;
      _count = 0;
    }
    
    void update(void) {
      switch(_currentMode) {
        case FirstPause:
        case SecondPause:
          _pixelRing->run();
          delay(500);
          _count++;
          if (_count == 16) {
            if (_currentMode == FirstPause) {
              _currentMode = FirstCalibrate;
            } else {
              _currentMode = SecondCalibrate;
            }
            _count = 0;
            _pixelRing->changeState(RING_BLUE_FADE);
          }
          break;
          
        case FirstCalibrate:
        case SecondCalibrate:
          _pixelRing->run();
          _surfaceSensors->calibrate();
          _count++;
          if (_count == 200) {
            if (_currentMode == FirstCalibrate) {
              _currentMode = SecondPause;
            _pixelRing->changeState(RING_GREEN_COUNTDOWN);
            } else {
              _currentMode = Complete;
            }
            _count = 0;
          }
          break;
          
        case Complete:
          taskManager.removeTask(_taskToken);
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

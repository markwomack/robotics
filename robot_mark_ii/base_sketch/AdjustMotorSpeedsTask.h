//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef ADJUSTMOTORSPEEDSTASK_H
#define ADJUSTMOTORSPEEDSTASK_H

// My includes
#include <Task.h>

// Local includes
#include "MotorsAndEncoders.h"

class AdjustMotorSpeedsTask : public Task {
  public:
    AdjustMotorSpeedsTask() {};

    void setMotorsAndEncoders(MotorsAndEncoders* motorsAndEncoders) {
      _motorsAndEncoders = motorsAndEncoders;
    };

    void update(void) {
      _motorsAndEncoders->adjustSpeeds();
    }

  private:
    MotorsAndEncoders* _motorsAndEncoders;
};
#endif //ADJUSTMOTORSPEEDSTASK_H

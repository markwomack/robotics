//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef ADJUSTANIMATIONTASK_H
#define ADJUSTANIMATIONTASK_H

// My includes
#include <Task.h>

// Local includes
#include "Animation.h"

class AdjustAnimationTask : public Task {
  public:
    AdjustAnimationTask() {};

    void setAnimation(Animation* animation) {
      _animation = animation;
    };

    void update(void) {
      _animation->update();
    }

  protected:
    Animation* _animation;
};
#endif  // ADJUSTANIMATIONTASK_H

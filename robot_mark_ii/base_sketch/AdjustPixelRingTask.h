//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef ADJUSTPIXELRINGTASK_H
#define ADJUSTPIXELRINGTASK_H

// My includes
#include <Task.h>

// Local includes
#include "PixelRing.h"

class AdjustPixelRingTask : public Task {
  public:
    AdjustPixelRingTask() {};

    void setPixelRing(PixelRing* pixelRing) {
      _pixelRing = pixelRing;
    };

    void update(void) {
      _pixelRing->run();
    }

  protected:
    PixelRing* _pixelRing;
};
#endif  //ADJUSTPIXELRINGTASK_H

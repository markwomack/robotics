//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef READSURFACESENSORSTASK_H
#define READSURFACESENSORSTASK_H

// My includes
#include <Task.h>

// Local includes
#include "SurfaceSensors.h"

class ReadSurfaceSensorsTask : public Task {
  public:
    ReadSurfaceSensorsTask() {};

    void setSurfaceSensors(SurfaceSensors* surfaceSensors) {
      _surfaceSensors = surfaceSensors;
    }
    
    void update(void) {
      // read raw sensor values
      _surfaceSensors->readCalibrated();
    };

  private:
    SurfaceSensors* _surfaceSensors;
};
#endif // READSURFACESENSORSTASK_H

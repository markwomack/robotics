//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef READEDGESENSORSTASK_H
#define READEDGESENSORSTASK_H

// My includes
#include <Task.h>

// Local includes
#include "EdgeSensors.h"

class ReadEdgeSensorsTask : public Task {
  public:
    ReadEdgeSensorsTask() {};

    void setEdgeSensors(EdgeSensors* edgeSensors) {
      _edgeSensors = edgeSensors;
    }
    
    void update(void) {
      // read raw sensor values
      _edgeSensors->read();
    };

  private:
    EdgeSensors* _edgeSensors;
};
#endif // READEDGESENSORSTASK_H

//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//
 
#ifndef NETWORK_HUB_H
#define NETWORK_HUB_H

#include <Udp.h>

// This is a work in progress, just the minimum of what
// is needed for the network for now. This could be the
// generic definition for specific implementations
// (ie wifi, ethernet, etc)

class NetworkHub {
  public:
    NetworkHub() { /* Nothing to see here, move along. */ }

    int start(void);

    UDP* getUdpPort(unsigned int portNum);
    
};

#endif

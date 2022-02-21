/*
 *      Please see the README.md for a full description of this program and
 *      project.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */
 
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

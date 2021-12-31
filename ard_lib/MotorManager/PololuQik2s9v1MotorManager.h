/*
 *      Please see the README.txt for a full description of this program and
 *      project.

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

#ifndef PololuQik2s9v1MotorManager_h
#define PololuQik2s9v1MotorManager_h

#include <inttypes.h>
#include <PololuQik.h>
#include "MotorManager.h"

class PololuQik2s9v1MotorManager : public MotorManager
{

public:
  PololuQik2s9v1MotorManager(uint8_t txPin, uint8_t rxPin, uint8_t resetPin);

  void setMotorSpeed(uint8_t motor, int speed);
  void setMotorSpeeds(int speedLeft, int speedRight);
  
private:
  PololuQik2s9v1* qik;
};

#endif

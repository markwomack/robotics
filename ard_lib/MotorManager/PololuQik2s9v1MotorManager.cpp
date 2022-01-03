/*
 *      Please see the README.txt for a full description of this program and
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

 #include "PololuQik2s9v1MotorManager.h"
  
 PololuQik2s9v1MotorManager::PololuQik2s9v1MotorManager(
   uint8_t txPin, uint8_t rxPin, uint8_t resetPin)
{
  qik = new PololuQik2s9v1(txPin, rxPin, resetPin);
  qik->init(9600);
}

int mapSpeed(double input) 
{
  // Pin to value between -1 and 1, then multiply by 127
  // for expected qik value.
  return (int)(min(max(input, -1.0), 1.0) * 127);
}

void PololuQik2s9v1MotorManager::setMotorSpeed(uint8_t motor, double speed)
{
  if (motor == LEFT_MOTOR) {
    qik->setM0Speed(mapSpeed(speed));
  } else {
    qik->setM1Speed(mapSpeed(speed));
  }
}

void PololuQik2s9v1MotorManager::setMotorSpeeds(double speedLeft, double speedRight)
{
  qik->setSpeeds(mapSpeed(speedLeft), mapSpeed(speedRight));
}

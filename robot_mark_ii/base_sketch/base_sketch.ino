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

// Base Arduino includes
#include <Arduino.h>
#include <inttypes.h>

// My includes
#include <ButtonExecutor.h>
#include <DebugMsgs.h>
#include <UDPPrintWrapper.h>

// Local includes
#include "pin_assignments.h"
#include "helper_methods.h"
#include "network_hub.h"
#include "behaviors.h"

PixelRing* pixelRing;

// This is the behavior defined for the robot
Behavior* behavior;

// This is the hub that connects to the wider network
NetworkHub networkHub;

// Create the button executor that will start/stop execution when button
// is pressed.
ButtonExecutor buttonExecutor;

void setup() {
  Serial.begin(9600);
  
  // Replace the line below with this one to disable all debugging messages
  DebugMsgs.disableAll();
  DebugMsgs.enableLevel(DEBUG);

  // Initialize the pixel ring
  pixelRing = initializePixelRing();
  pixelRing->start(RING_OFF);

  // Set up remote udp port debugging
  if (UDP_DEBUGGING) {
    if (networkHub.start() == 0) {
      DebugMsgs.println("Switching to debug messages through remote udp");
      
      UDPPrintWrapper* udpPrint =
        new UDPPrintWrapper(networkHub.getUdpPort(DEBUG_UDP_PORT), UDP_TARGET_ADDRESS, UDP_TARGET_PORT);
      
      DebugMsgs.setPrint(udpPrint);
      DebugMsgs.println("Starting debug messages through remote udp");
    } else {
      DebugMsgs.println("Error connecting to network, debug messages will continue local");
    }
  }
  
  // Setup the executor with the button pin and callbacks
  buttonExecutor.setup(
    BUTTON_PIN, HIGH, setupCallback, startCallback, stopCallback, idleCallback);
}

void loop() {
  // Allow the executor to perform a loop call to check for button presses
  // and to execute the sketch callbacks
  buttonExecutor.loop();
}

// This is where the sketch should setup one time settings like pin modes.
// It will be called just once when the executor is setup.
void setupCallback() {
  DebugMsgs.println("Sketch setup");

  pixelRing->changeState(RING_WHITE_FADE);

  // Create the behavior
  behavior = createBehavior();
}

// This is where the sketch should set initial state before execution.  It will
// be called every time the button is pushed to start execution, before
// execution is started.
void startCallback() {
  DebugMsgs.println("Sketch start");

  // Start the behavior
  behavior->start(&buttonExecutor, pixelRing);
}

// This is where the sketch should handle the ending of execution.  It will be
// called whenever execution is ended (button push or request to abort
// execution).
void stopCallback() {
  DebugMsgs.println("Sketch stop");

  // Stop the behavior
  behavior->stop();

  pixelRing->changeState(RING_WHITE_FADE);
}

// Called when the executor is not executing.
void idleCallback() {
  pixelRing->run();
}

void abortExecution() {
  buttonExecutor.abortExecution();
}

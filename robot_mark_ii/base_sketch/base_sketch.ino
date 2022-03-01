//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// Base Arduino includes
#include <Arduino.h>
#include <inttypes.h>

// My includes
#include <TaskManager.h>
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

// Create the taskManager that will start/stop execution when button
// is pressed.
TaskManager taskManager;

void setup() {
  Serial.begin(9600);
  
  DebugMsgs.disableAll();
  // Uncomment this line to enable debug msgs
  //DebugMsgs.enableLevel(DEBUG);

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
  
  // Setup the task manager with the button pin and callbacks
  taskManager.setup(
    BUTTON_PIN, HIGH, setupCallback, startCallback, stopCallback, idleCallback);
}

void loop() {
  // Allow the task to perform a loop call to check for button presses
  // and to execute the sketch callbacks
  taskManager.loop();
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
  behavior->start(&taskManager, pixelRing);
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

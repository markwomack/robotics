//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// Base Arduino includes
#include <Arduino.h>
#include <inttypes.h>

// My includes
#include <TaskManager.h>
#include <Task.h>
#include <DebugMsgs.h>
#include <UDPPrintWrapper.h>

// Local includes
#include "pin_assignments.h"
#include "globals.h"
#include "NetworkHub.h"
#include "DistanceSensors.h"
#include "EdgeSensors.h"
#include "MotorsAndEncoders.h"
#include "SurfaceSensors.h"
#include "Animation.h"
#include "BehaviorTask.h"
#include "StayOnTableTopTask.h"
#include "PushOffTableTopTask.h"
#include "AdjustAnimationTask.h"
#include "CalibrateSurfaceSensorsTask.h"

// Simple idle task to fade the pixel ring white
class IdleTask : public AdjustAnimationTask {
  public:
    IdleTask() {};
    
    void start(void) {
      _animation->setAnimationState(WHITE_FADE);
    };
};
IdleTask idleTask;

// All of the sensors, motors, and pixel ring
EdgeSensors edgeSensors;
DistanceSensors distanceSensors;
SurfaceSensors surfaceSensors;
MotorsAndEncoders motorsAndEncoders;
Animation animation;

// This is the behavior defined for the robot
BehaviorTask* behaviorTask;

// This is the hub that connects to the wider network
NetworkHub networkHub;

// Creates the behavior task to be executed
BehaviorTask* createBehaviorTask(void) {
  return new CalibrateSurfaceSensorsTask();
  //return new PushOffTableTopTask();
  //return new StayOnTableTopTask();
}

void setup() {
  Serial.begin(9600);
  
  DebugMsgs.disableAll();
  // Uncomment this line to enable debug msgs
  DebugMsgs.enableLevel(DEBUG);
   
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

  // These are globals that need to be initialized
  // before the rest of the program
  edgeSensors.initialize();
  distanceSensors.initialize();
  surfaceSensors.initialize();
  motorsAndEncoders.initialize();
  animation.initialize();

  // get the behavior task to execute
  behaviorTask = createBehaviorTask();

  idleTask.setAnimation(&animation);
  
  // add tasks into the task manager
  taskManager.addIdleTask(&idleTask, 50);
  behaviorTask->setTaskToken(taskManager.addTask(behaviorTask, 10));

  // start monitoring the button to start the tasks
  taskManager.startMonitoringButton(BUTTON_PIN, HIGH);
}

void loop() {
  // Execute tasks
  taskManager.update();
}

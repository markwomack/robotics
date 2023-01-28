# Base Sketch

This is the base sketch for the Mark II 'Beatrice' robot that supports all of the behaviors
it has been programmed to perform.

## Files

### base_sketch.ino
<p>This sketch is the base of the program. It sets up all of the sensors, motors,
and leds that will be used on the robot. It creates the specific BehaviorTask that
will be executed, and uses the Task Manager to monitor a button to start/stop the
robot execution.</p>

### robot_constants.h
<p>This file contains the constants related to the robot, mostly physical constants
like the number of encoder ticks per wheel revolution, etc.</p>

### pin_assignments.h
<p>This file contains all of the Teensy 3.5 pin assignments used throughout the
code.</p>

### Behaviors
<p>The robot can be programmed to execute 'behaviors'. These behaviors can be anything
that it can use its sensors and output controls to perform.</p>

#### BehaviorTask
<p>The BehaviorTask is the base class for all of the behaviors that can be executed
on the robot. It has common members and methods that can be used by all subclasses
to implement their specific behavior.</p>

#### StayOnTableTopTask
<p>The StayOnTableTopTask is the behavior that will keep the robot on the table and
not fall off. Once it finds a table edge, the robot will go back and forth on the table.
It passes
[Phase 1 of the HBRC TableTop Challenge](https://www.hbrobotics.org/index.php/challenges/).</p>

#### PushOffTableTopTask
<p>The PushOffTableTopTask is the behavior that will search for an object, and when found
it will proceed to push the object off of the table. After pushing off the table, the
robot will do a happy dance and then wait to be activated again. It passes
[Phase 2 of the HBRC TableTop Challenge](https://www.hbrobotics.org/index.php/challenges/).</p>

### Sensors and Controls
<p>The robot has a number of sensors and output controls that it can use from its behaviors.</p>

#### EdgeSensors
<p>Edge sensors are used to detect the edges of the table, and are the four IR sensors located on
the four arms that come from the corners of the robot. The EdgeSensor class defines the methods
that can be used to initialize and access the sensor values.</p>

#### DistanceSensors
<p>Distance sensors are the ten sensors that ring the robot, two sets of 5. These are ToF
sensors that return a distance when an object is close. The DistanceSensors class defines the
methods that can be used to initialize and access the sensor values.</p>
  
#### MotorsAndEncoders
<p>There are two motors on the robot, each with its own encoders to know how far a wheel has
turned and thus how far the robot has travelled. The MotorsAndEncoders class defines the
methods that can be used to initialize and control the motors and encoders.</p>

#### PixelRing
<p>The robot has an Adafruit NeoPixel ring installed that can be used to indicate state
by the behavior implementation. The PixelRing class defines the methods and to
initialize and control the pixel ring.</p>

#### SurfaceSensors
<p>There are 10 surface sensors on the bottom of the robot. These are meant to detect things
on the surface of the table, such as lines of black tape. The SurfaceSensors class defines
the methods to initialize and access the sensor values.</p>

### Support Tasks
<p>There are a number of tasks used by the behavior tasks to help in reading sensors or
updating output controls. Each behavior task implementation can install whichever support
tasks it needs.</p>

#### AdjustMotorSpeedTask
<p>The AdjustMotorSpeedTask is called periodically to allow the motor PID controls, found
in the MotorsAndEncoders class, to adjust the power to the motors so that the target
speed is maintained.</p>

#### AdjustPixelRingTask
<p>Many of the defined patterns that can be shown on the PixelRing are animated. The
AdjustPixelRingTask allows the PixelRing to update its LEDs.</p>

#### ReadEdgeSensorsTask
<p>The ReadEdgeSensorsTask allows the robot to periodically read the values of the
edge sensors.</p>

### Miscellaneous

#### NetworkHub
<p>Optionally, the robot can be connected to WiFi to aid in debugging. The NetworkHub class
is used to connect to a WiFi network. The WiFi netword name and login credentials are stored
in the file secrets.h.</p>

# Design Robot Mark II - 'Beatrice'

This document outlines the design of the Mark II 'Beatrice' robot.

## Components

<p>These are the major components that make up Beatrice. This list does not have incidentals like protoboards, sockets, pins,
spacers, connectors, and wires. A wiring diagram is be provided, but how the components and wiring is arranged is left as
an exercise to the reader.</p>

- [Teensy 3.5 microcontroller](https://www.pjrc.com/store/teensy35.html)
- [Pololu Qik 2s9V1 motor controller](https://www.pololu.com/product/1110)
- [Polulu Micro metal gearmotor 12v 250:1 (2)](https://www.pololu.com/product/3055)
- [Pololu magnetic motor encoders (2)](pololu.com/product/2598)
- [Pololu 32x7mm wheels (2)](https://www.pololu.com/product/1087)
- [Pololu Gearmotor brackets (2)](https://www.pololu.com/product/1089)
- [Pololu Mini pushbutton power switch](https://www.pololu.com/product/2808)
- [Pololu 5v step-up/step-down voltage regulator](https://www.pololu.com/product/2119)
- [Pololu 7.5v step-up voltage regulator](https://www.pololu.com/product/4014)
- [Pololu IR sensors (4 total for edge detection](https://www.pololu.com/product/2459)
- [Poloul IR sensors (2 total for line detection](https://www.pololu.com/product/961)
- [Pololu VL6180X Time-of-Flight Distance Sensor (10)](https://www.pololu.com/product/2489)
- [Pololu mini pushbutton switch (2)](https://www.pololu.com/product/1400)
- [Adafruit NeoPixel ring 16x5050](https://www.adafruit.com/product/2855)
- [5v power bank battery](https://www.amazon.com/gp/product/B07K6HJTK2)
- [USB cable slim connector](https://www.amazon.com/gp/product/B01MZ8RA3A)
- [Pololu mini pushbutton switch (2)](https://www.pololu.com/product/1400)
- [Adafruit AirLift ESP32 WIFI Co-Processor](https://www.adafruit.com/product/4201)

### Chassis

<p>The chassis is a collection of 3D printed parts for the mounted sensors, a couple of custom PCB boards, and several protoboards
  all put together to form the body of the robot. The chassis is designed to be symmetrical along the axis of the differential drive
  wheels. The profile and sensors of the robot are the same, front to back, flipping on the wheel axis. One side has been arbritrarily
  chosen to be the 'front', but future coding could treat each side equally, eliminating any need for the robot to turn around,
  instead just switching the 'front' in the software.</p>
  
### Microcontroller

<p>The Teensy 3.5 is the heart of the robot. It drives all of the sensors and behaviors.</p>
<p>The Teensy has been modified to use it's own power supply instead of relying on power from the USB port. This is [documented
  here](https://www.pjrc.com/store/teensy35.html) on the back side of the pinout card (see the "VIN separated from VUSB" note). 
  Be sure to do this when assembling the robot.</p>
<p>As of today the Teensy 3.5 may be discontinued (though I have been able to find it at a local MicroCenter). The newer
  Teensy 4.1 is almost a drop in replacement, however there are two issues to be aware of. First, while the Teensy 4.1 has the
  same pin form factor, but the pins are arranged differently. You will have to take this into account when wiring, and the 
  pin_assignments.h file in the code will need to be updated accordingly. Second, the Teensy 4.1 pins are NOT 5v tolerant as 
  they are for the 3.5. You will have to use 3.3v for all of the connections, and will need to have a 3.3v voltage regulator
  instead of the 5v voltage regulator. [Pololu provides a 3.3v voltage regulator](https://www.pololu.com/product/2122).
  Though I have not tried it as of yet, all of the electronic components should function correctly using 3.3v instead of 5v.</p>

### Motor controller

<p>The Pololu Qik 2s9v1 motor controller is a simple to use controller that can drive two motors. It uses serial to receive
  motor commands, and one of the Teensy serial ports is dedicated to the Qik. While the controller run on 5v, it provides
  a separate power source to the motors, which on this robot is 7.5v (see Power section below).</p>

### Motors and encoders

<p>The Pololu micro metal gearmotors can handle up to 12v, but Beatrice runs them with just 7.5v. The motors have a 250:1
gear ratio, which will provide a lot of torque to the wheels to help Beatrice get over bumps and ridges. With the mini 
magnetic encoders having 12 ticks per shaft revolution with the 250:1 gear ratio, there will be 3000 ticks per wheel
revolution. This will provide a high level of accuracy in the odometry.</p>
<p>Beatrice uses a differential drive centered in the chassis. This allows the robot to move forward, backward, turn, and
  spin in place.</p>

### Power

<p>The power to Beatrice is provied by a 5v power bank connected via a modified USB cable to the 5v pushbutton power switch.
The power bank can provide up to 2A of current. The pushbutton switch provides reverse voltage protection. The power flows to
the robot from the switch to two different voltage regulators. The 5v regulator provides power to all of the electronics:
the Teensy, the Qik, and all of the sensors. The 7.5v regulator steps up the 5v to 7.5v to provide more power to the motors.
The 7.5 regulator is connected to the Qik motor controller, which passes the power to the motors.</p>
<p>The 5v regulator can deliver up to 1A in current which is more than sufficient for the electronics. The 7.5v regulator
can provide up to 3A current which much greater than the motor stall current of .75A (1.5A for both motors combined).</p>

### WIFI (optional, but recommended)

<p>Beatrice connects to the internet via the Adafruit ESP32 WIFI Co-Processor breakout board. It is connected to the Teensy
  through the SPI interface, and a custom Adafruit version of the Arduino WiFiNINA library is used to allow different pin
  locations. The internet connection is used to send debugging messages to a remote console, which greatly aids in the
  writing and debugging of robot behaviors. It is much harder to sense objects or program movement with a USB cable
  connected to the back of the robot's Teensy. However, this portion of the build is optional, but greatly recommended.
  Please see the ArduinoLogging library for an example of a setup on a remote computer to receive messages from the robot.</p>

### Sensors

<p>Beatrice is outfitted with different sets of sensors, each for a different purpose.</p>

#### Edge Sensors

<p>Extending out from each corner of the robot are 4 Pololu IR sensors, one for each corner. These are meant to detect
  edges of the tabletop on which the robot is operating. There is no special calibration performed for these sensors.
  Once the value goes above a certain threshold and 'edge' is detected. Each sensor is directly connected to a pin on
  the Teensy, which reads the value periodically when required by the behavior.</p>

#### Distance Sensors

<p>Wrapping around the robot are 10 VL6180X Time-of-Flight distance sensors (3 front, 3 rear, 2 right, 2 left). These
  are meant to detect objects in proximity to the robot. The maximum range of the VL6180X is 60cm, but in practice it
  is more in the 20-30cm range. Each sensor is an I2C device, and values are requested and read using the I2C bus.
  Unfortunately, each sensor hasthe same I2C address, which in turn requires a Multiplexer chip to select the specific
  sensor before interacting with it. For Beatrice this is done using a custom PCB board that is designed to have the
  multiplexer chip and 5 sensors mounted directly on the board. The board is then incorporated into the body of
  the chassis. There is one PCB for the front, and one PCB for the rear, with matching configurations. A separate
  I2C bus and set of selector pins on the Teesny are used to control each board in turn. While the sensors could be
  configured to run in 'continous' mode, currently the code samples distance measurements as needed for the specific
  task in hand. This avoids a periodic task running over its alloted time to execute, and overlapping signals each
  sensor sends out to measure distance. Future code could be written to better time the measurments for each
  sensor so that they do not intefere with each other.</p>

#### Line Tracking Sensors

<p>Underneath the robot, mounted on a 3D printed under-carriage, are more Pololu IR sensors. These are meant to detect
  line patterns on the tabletop or a maze. There are 10 sensors in total, arranged in lines: 2 at the front, 6 across
  the center, and 2 more at the rear. This arrangement makes detecting horizontal and vertical line patterns more
  efficient. Similar to the edge sensors, each sensor is connected to a pin on the Teensy. However, unlike the edge
  sensors, the line sensors require some level of calibration before use to detect lines. While calibrating, the robot
  needs to be moved over the lightest and darkest values found on the tabletop or maze. Once the calibration is completed,
  the calibrated values will then be used in detecting lines.</p>

### Robot Control

<p>The 5v pushbutton power switch will turn Beatrice on and off, but there are is another button to actually control
  the start and stop of the robot behavior. Beatrice uses the TaskManager library to monitor a pin on the Teensy that
  is connected to this pushbutton. When first turned on Beatrice will be in idle mode, with the PixelRing pulsing white.
  When the pushbutton is pressed, the programmed behavior will start. When the pushbutton is pressed again, the behavior
  will be stopped. Pushing the button again will start the behavior, now initialized to forget the previous execution.</p>
<p>Because Beatrice can perform several different behaviors, there is another push button that can be used to select the
  behavior to execute next time the start button is pressed. Each behavior type is numbered, and when selecting, the
  selected behavior number is displayed on the PixelRing by the matching number of LEDs.</p>
  
### State display

<p>Beatrice uses the Adafruit PixelRing to display its current state. When idle it will pulse white. While running it will
  update the PixelRing as it executes the behavior.

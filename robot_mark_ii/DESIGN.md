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

<p>The VL6180X ToF distance sensors are mounted on a custom PCB board that is designed to use a multiplexer to choose which sensor
to access on the I2C bus. This is required since the sensors all have the same I2C address.</p>

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
  instead of the 5v voltage regulator. [Pololu provides a 3.3v voltage regulator](https://www.pololu.com/product/2122).</p>
  Though I have not tried it as of yet, all of the electronic components should function correctly using 3.3v instead of 5v.

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
The power bank can provide up to 2A current. The pushbutton switch provides reverse voltage protection. The power flows to
the robot from the switch to two different voltage regulators. The 5v regulator provides power to all of the electronics:
the Teensy, the Qik, and all of the sensors. The 7.5v regulator steps up the 5v to 7.5v to provide more power to the motors.
The 7.5 regulator is connected to the Qik motor controller, which passes the power to the motors</p>
<p>The 5v regulator can deliver up to 1A in current which is more than sufficient for the electronics. The 7.5v regulator
can provide up to 3A current which much greater than the motor stall current of .75A (1.5A for both motors combined).</p>

### Sensors
[COMING SOON]

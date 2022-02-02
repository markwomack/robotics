# Design Robot Mark II - 'Beatrice'

This document outlines the design of the Mark II 'Beatrice' robot.

## Components

These are the major components that make up Beatrice. This list does not have incidentals like protoboards, sockets, pins, and wires.

- [Teensy 3.5 microcontroller](https://www.pjrc.com/store/teensy35.html)
- [Pololu Qik 2s9V1 motor controller](https://www.pololu.com/product/1110)
- [Polulu Mini metal gearmotor 12v 250:1 (2)](https://www.pololu.com/product/3055)
- [Pololu magnetic motor encoders (2)](pololu.com/product/2598)
- [Pololu 32x7mm wheels (2)](https://www.pololu.com/product/1087)
- [Pololu Gearmotor brackets (2)](https://www.pololu.com/product/1089)
- [Pololu Mini pushbutton power switch](https://www.pololu.com/product/2808)
- [Pololu 5v step-up/step-down voltage regulator](https://www.pololu.com/product/2119)
- [Pololu 7.5v step-up voltage regulator](https://www.pololu.com/product/4014)
- [Pololu IR sensors (4 total for edge detection](https://www.pololu.com/product/2459)
- [Poloul IR sensors (2 total for line detection](https://www.pololu.com/product/961)
- [Pololu VL6180X Time-of-Flight Distance Sensor (10)](https://www.pololu.com/product/2489)
- [Adafruit NeoPixel ring 16x5050](https://www.adafruit.com/product/2855)
- [5v power bank battery](https://www.amazon.com/gp/product/B07K6HJTK2)

The VL6180X ToF distance sensors are mounted on a custom PCB board that is designed to use a multiplexer to choose which sensor
to access on the I2C bus. This is required since the sensors all have the same I2C address.

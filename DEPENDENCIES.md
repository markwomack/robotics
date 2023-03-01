# Dependencies

## Third Party Dependencies
The sketches and libraries have dependicies on some third party libraries.

- [MotorAndEncoderManager](https://github.com/markwomack/MotorAndEncoderManager) - Used
  drive the motors and encoders on the robots.
- [Pololu Qik controller](https://github.com/pololu/qik-arduino) - One of the motor
  controllers used.
- [Pololu VL6180X sensors](https://github.com/pololu/vl6180x-arduino) - The distance
  sensors used.
- [Pololu QTR sensors](https://github.com/pololu/qtr-sensors-arduino) - The edge and
  surface sensors used.
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) - The hardware used
  to communicate state.
- [Adafruit WiFiNINA](https://github.com/adafruit/WiFiNINA) - WiFi connction used for
  debugging purposes<br/>
**^^ MUST ^^** use the above version to allow different pin locations
- [PID controller](https://github.com/br3ttb/Arduino-PID-Library) - Used to precisely
  control the motor speeds.
- [PixelRingAnimator](https://github.com/markwomack/PixelRingAnimator) - Provides
  animations to run on the NeoPixel ring.
- [ArduinoLogging](https://github.com/markwomack/ArduinoLogging) - Used to print debug
  messages.
- [TaskManager](https://github.com/markwomack/TaskManager) - Used to manage the various
  tasks executed to run the robot behaviors.

Some of these libraries might already be installed in the Arduino
IDE, so check first.

## Local Dependencies
In addition to the above libraries, there are a number of 'local'
libraries that should also be installed to the Arduino environment.
They are located in the [ard_lib directory](https://github.com/markwomack/robotics/tree/main/ard_lib),
and can be copied directly to the Arduino library directory.

- [VL6180I2CMux](https://github.com/markwomack/robotics/tree/main/ard_lib/VL6180I2CMux) -
  The VL6180X ToF sensors used on Beatrice are accessed through I2C, but the address is
  hardcoded. This library allows for a multiplexer to be used to access each sensor as
  if it was the only one. There is 
  [specific hardware designed](https://github.com/markwomack/robotics/tree/main/robot_mark_ii/physical)
  to implement this feature and this code provides the methods to access it.

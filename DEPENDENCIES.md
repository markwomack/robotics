# Dependencies

## Third Party Dependencies
The sketches and libraries have dependicies on some third party libraries.

- [MotorAndEncoderManager](https://github.com/markwomack/MotorAndEncoderManager)
- [Pololu Qik controller](https://github.com/pololu/qik-arduino)
- [Pololu VL6180X sensors](https://github.com/pololu/vl6180x-arduino)
- [Pololu QTR sensors](https://github.com/pololu/qtr-sensors-arduino)
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
- [Adafruit WiFiNINA](https://github.com/adafruit/WiFiNINA)<br/>
*^ **Must** use the above version to allow different pin locations
- [PID controller](https://github.com/br3ttb/Arduino-PID-Library)
- [PixelRingAnimator](https://github.com/markwomack/PixelRingAnimator)
- [ArduinoLogging](https://github.com/markwomack/ArduinoLogging)
- [TaskManager](https://github.com/markwomack/TaskManager)

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

# Dependencies

## Third Party Dependencies
<p>The sketches and libraries have dependicies on some third party libraries.</p>

- [Pololu Qik controller](https://github.com/pololu/qik-arduino)
- [Pololu VL6180X sensors](https://github.com/pololu/vl6180x-arduino)
- [Pololu QTR sensors](https://github.com/pololu/qtr-sensors-arduino)
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
- [Adafruit WiFiNINA](https://github.com/adafruit/WiFiNINA)<br/>
*^ **Must** use the above version to allow different pin locations
- [PID controller](https://github.com/br3ttb/Arduino-PID-Library)
- [ArduinoLogging](https://github.com/markwomack/ArduinoLogging)
- [TaskManager](https://github.com/markwomack/TaskManager)

<p>Some of these libraries might already be installed in the Arduino
IDE, so check first.</p>

## Local Dependencies
<p>In addition to the above libraries, there are a number of 'local'
libraries that should also be installed to the Arduino environment.
They are located in the ard_lib directory, and can be copied directly
to the Arduino library directory.</p>

- MotorController - Uses the PID controller software to control the
power to the motors to maintain a target speed.
- MotorManager - Uses the Qik controller library to implement an
interface to control the motors and access the encoders.
- VL6180I2CMux - The VL6180X ToF sensors used on Beatrice are accessed
through I2C, but the address is hardcoded. This library allows for a
multiplexer to be used to access each sensor as if it was the only
one. There is 
[specific hardware designed](https://github.com/markwomack/robotics/tree/main/robot_mark_ii/physical)
to implement this feature and this code provides the methods to access
it.

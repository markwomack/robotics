# Robot Mark II 'Beatrice'
<p>This is the directory that contains all of the code and designs specific to the
Robot Mark II 'Beatrice'.</p>

* Beatrice is an all-around robot with many different
sensor sets that allow it to detect table edges, objects that are close by,
and lines that are placed on a table or floor.

* It is a differential drive robot
with motor encoders. This means that it has software control for the motor power
(PID) so that the motors perform at an expected rate of speed.

* The framework provided from using the
[Task Manager library](https://github.com/markwomack/TaskManager) and implemented
support classes means that it can be easily extended to perform different
behaviors.

* Printing debug messages to a remote computer allows for basic feedback when
debugging new bahaviors. This is done by using UDP over WiFi with the
[Arduino Logging library](https://github.com/markwomack/ArduinoLogging).

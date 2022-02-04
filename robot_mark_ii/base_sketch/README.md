# Base Sketch

This is the base sketch for the Mark II 'Beatrice' robot that supports all of the behaviors
it has been programmed to perform.

## Files

### robot_constants.h

This file contains the constants related to the robot, mostly physical constants like the
number of encoder ticks per wheel revolution, etc.

### pin_assignments.h

This file contains all of the Teensy 3.5 pin assignments.

### base_sketch.ino

This sketch is the base of the program. It sets up the basic ButtonExecutor behavior around
the start button that is implemented in the hardware. It will manage the Teensy LED and
create/start/stop the behaviors.

### callback_context.h

This defines the struct that is passed between the callbacks used for the behavior. The
CallbackContext stores state across calls and access to the sensors in the callbacks.

### helper_methods.h/.cpp

Methods that help callback methods access the data and sensors they use.

### behaviors.h/.cpp

Defines and implements the behaviors that the Mark II 'Beatirice' robot uses for the
functions it demonstrates.

### pixel_ring.h/.cpp

Code used to drive the AdaFruit NeoPixel ring used to display state during behavior
execution.

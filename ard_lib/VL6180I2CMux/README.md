# VL6180I2CMux
<p>The VL6180I2CMux library provides code for accessing and managing the custom PCB used to manage up to 5 VL6180X Time-of-flight
  I2C distance sensors on an I2C bus. The VL6180X sensors use a single I2C address, and will conflict one another if placed on the
  same I2C bus. To remedy this situation, a simple multiplexer ship is used on the custom PCB to allow for a single sensor to be
  active at any time. Simply select the sensor you want to access before accessing it. In addition, the VL6180I2CMux library contains
  methods for accessing the sensor. However, once selected, you should be able to access the sensor on the I2C bus as you would normally.</p>
  

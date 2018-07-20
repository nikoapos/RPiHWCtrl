gpio package
============

The gpio package contain classes which can be used for controlling the GPIO pins
of the Raspberry Pi. There are two classes, the GpioInput, which can be used to
read binary input from a GPIO pin, and the GpioOutput, which can be used to
write binary output to a GPIO pin.

To see how to use the GpioInput and GpioOutput classes you can see the following
examples:

- [GpioInputExample](src/examples/GpioInputExample.cpp): Demonstrates how to
    use the GpioInput class
- [GpioOutputExample](src/examples/GpioInputExample.cpp): Demonstrates how to
    use the GpioOutput class
- [FunWithLEDs](src/examples/FunWithLEDs.cpp): An example of using multiple GPIO
    pins, both for input and output
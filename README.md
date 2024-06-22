# VL53L8 Sensor Example on Raspberry Pi

This repository contains a C implementation for using the ST VL53L8 sensor, including a basic example of ranging. The implementation uses the ST-provided ULD driver, which can be found [here](https://www.st.com/en/embedded-software/stsw-img040.html).

### Files

- `main.c`: Contains the main function and basic ranging example (modified Example_1_ranging_basic from ST).
- `platform.c` & `platform.h`: Platform-specific implementations for I2C communication.
- `vl53l8cx_api.c`, `vl53l8cx_api.h`, `vl53l8cx_buffers.h`: ST ULD driver files.
- `Makefile`: Makefile to compile the project & clean build files.

### Notes

- The implementation was written for Raspberry Pi but should work on other Linux-based systems with I2C support.
- Compile the project with Makefile
- Run with ./main

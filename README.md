# 507-Arm-Project

This repository holds the code, hardware design files, and documentation for a 4.5 degree-of-freedom robotic arm developed as a term project for ME 507 at Cal Poly, San Luis Obispo.

The robot is a semi-autonomous color-sorting arm. It uses four stepper-driven joints, a servo-powered gripper, a joystick for manual positioning, and a TCS34725 color sensor to detect and sort foam blocks by color.

The project documentation pages are available at:

https://andrewthegr8.github.io/507-Arm-Project/

## Project Documentation

The `docs` folder contains the Doxygen-generated project documentation and homepage content. The documentation describes the mechanical design, electrical design, software architecture, finite state machine, challenges, results, and future improvements for the robotic arm.

Please see the project documentation at: https://andrewthegr8.github.io/507-Arm-Project/

## Core Files

The `Core` folder contains the main STM32 project source files. This includes the main program loop, finite state machine logic, interrupt configuration, and peripheral initialization generated and modified through the STM32 development environment.

Key project files include:

* `main.c`, which initializes the system and runs the main control loop.
* `fsm.c`, which implements the finite state machine for robot operation.
* `motion.c`, which handles motor position commands and trajectory execution.
* `joystick.c`, which reads and filters joystick input.
* `servo.c`, which controls the gripper servo.
* `color_sensor.c`, which reads and classifies color sensor data.
* `maneuvers.c`, which defines the robot’s autonomous trajectories.

## Driver Files

The `Drivers` folder contains STM32 HAL drivers and additional hardware interface code used by the project. These files support communication with the STM32H7 microcontroller peripherals and external components such as the motion planning chips and color sensor.

## Hardware Design

The project uses a custom PCB designed for the STM32H7 microcontroller, stepper motor drivers, TMC429 motion controllers, power regulation, debugging, and peripheral connections. The PCB was designed to support the robotic arm’s motors, joystick, color sensor, and servo-controlled gripper.

## Firmware

The firmware is written in C using the STM32 ecosystem. The system uses joystick-controlled motion for manual block positioning and autonomous trajectories for color-based block sorting. A finite state machine coordinates startup, joystick control, color detection, and pick-and-place motion.

## Authors

Ethan Dickson, Andrew Jones, and Sara Chamness


# Project Overview
Authors: Ethan Dickson, Andrew Jones, & Sara Chamness

## Introduction

- Autonomous block sorting
- Color sensing
- Trajectory planning
- Servo gripper control
- C (not C++)

## Hardware

### Mechanical Design

Super cool stuff

### Electrical Design

- PCB
- STM32H7
- TMC429 Motion Controller
- TCS34725 Color Sensor
- Servo Gripper

## Software Architecture

- drivers
- open-source component libraries
- wrappers / HALs
- main/fsm?

<!-- ### Finite State Machine 

Description of robot states. -->

### Motion Control

Joystick control
Automated motion
- Waypoints and execute trajectory
required setup for zeroing since we don't have limit switches

### Color Detection

Description of sensor processing and classification.

## Mathematical Modeling and Sensor Interpretation

## Results

- motor motion
- color sensing

## Future Improvements

- limit switches
- object detection?
- greater range of motion with the joystick?

## References

1. TMC429 Datasheet
2. STM32H7 Reference Manual
3. TCS34725 Datasheet
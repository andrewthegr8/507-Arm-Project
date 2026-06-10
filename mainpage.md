# Project Overview
Authors: Ethan Dickson, Andrew Jones, & Sara Chamness

## Introduction

This website documents the complete design of a 4.5 DOF robotic arm. Our team created this robot for our term project in ME 507 at Cal Poly, San Luis Obispo.

The robot is a semi autonomous color-sorting device. The arm consists of four joints mobilized by stepper motors and a gripper controlled by a servo motor. It was designed to pick up small foam blocks and sort them by color. To operate the device, the user manipulates a joystick to direct the robot to the block, then presses the select button on the joystick. A color sensor mounted on the end effector reads the color and the robot autonomously brings the block to a predetermined location based on its color.

The system is built around a custom-designed PCB featuring an STM32H7 microcontroller. The project was developed using the STM32 ecosystem, and the firmware was implemented in C. The project utilizes open-source hardware drivers for interfacing with more complex electronics components, as well as the STM HAL library to work simplify communication with the microcontroller.

Photo!!!

## Hardware

### Mechanical Design

photo(s)
- 3D printed components
    - any special fits, design choices, etc. maybe link to (repo wiht) CAD files??
- end effector
- stepper sizing and selection (spreadsheet and calcs)
- servo
- power supply
- wiring (connectors, cables, etc.)
- wiring diagrams?

The robot arm was designed in SolidWorks and 3D printed out of PLA at the Cal Poly Makerspace. This minimized manufacturing costs and complexity for a custom design and allowed for easy iteration. The arm houses the four stepper motors at each of the joints, as well as the servo motor on the end effector. The end effector uses small gears to open the gripper, and the color sensor is mounted on the end effector. With the motors installed, the arm should be mounted on a flat surface to ensure stability. (do we use any screws? or is it all press fits?)

The stepper motors (numbered 1-4) were sized by calculating the anticipated dynamic load on each motor. Motor 1 must bear the greatest load since it supports the weight and motion of the other motors and accessory components.

| Required Motor Torques (Ncm) |            |             |             |              |             |
|------------------------------|------------|-------------|-------------|--------------|-------------|
| Motor                        | Static     | Dynamic     | Total       | FS (holding) | FS (curve)  |
| T0                           | 0          | 36.0690827  | 36.0690827  | 4.491381202  | 4.05471914  |
| T1                           | 167.195754 | 36.0690827  | 203.2648367 | 1.593979585  | 1.106930267 |
| T2                           | 68.52285   | 12.12007253 | 80.64292253 | 2.008855767  | 1.813550345 |
| T3                           | 14.95044   | 1.518964704 | 16.4694047  | 1.420816382  | -           |

### Electrical Design

- PCB
- STM32H7
- TMC429 Motion Controller
- TCS34725 Color Sensor
- Servo Gripper
- overdesigned - power, # of motors, USB, limit switches

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
- more motors with the extra motor drivers?

## References

1. TMC429 Datasheet
2. STM32H7 Reference Manual
3. TCS34725 Datasheet
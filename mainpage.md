# Project Overview
Authors: Ethan Dickson, Andrew Jones, & Sara Chamness

## Introduction

This website documents the complete design of a 4.5 DOF robotic arm. Our team created this robot for our term project in ME 507 at Cal Poly, San Luis Obispo.

The robot is a semi autonomous color-sorting device. The arm consists of four joints mobilized by stepper motors and a gripper controlled by a servo motor. It was designed to pick up small foam blocks and sort them by color. To operate the device, the user manipulates a joystick to direct the robot to the block, then presses the select button on the joystick. A color sensor mounted on the end effector reads the color and the robot autonomously brings the block to a predetermined location based on its color.

The system is built around a custom-designed PCB featuring an STM32H7 microcontroller. The project was developed using the STM32 ecosystem, and the firmware was implemented in C. The project utilizes open-source hardware drivers for interfacing with more complex electronics components, as well as the STM HAL library to work simplify communication with the microcontroller.

\image html RobotWithBlocks.png "Top View of Robot with Blocks"

## Hardware



### Mechanical Design

\image html CAD-ISO.png "Isometric View of CAD"


photo(s)
- 3D printed components
    - any special fits, design choices, etc. maybe link to (repo wiht) CAD files??
- end effector
- stepper sizing and selection (spreadsheet and calcs)
- servo
- power supply
- wiring (connectors, cables, etc.)
- wiring diagrams?

The robot arm was designed in SolidWorks and 3D printed out of PLA at the Cal Poly Makerspace. 3D printing minimized manufacturing costs and complexity and allowed for easy iteration. The arm houses the four stepper motors at each of the joints, as well as a servo motor on the end effector. The steppers are mounted to the fixed portion of each joint using screws and the shaft is press fit into the rotating component. The end effector uses small 3D-printed gears to open the gripper, and the color sensor is mounted on the end effector. The arm is mounted on plywood for stability.

The stepper motors (numbered 1-4) were sized by calculating the anticipated dynamic load on each motor. Motor 1 must bear the greatest load since it supports the weight and motion of the other motors and accessory components.

| Required Motor Torques (Ncm) |            |             |             |              |             |
|------------------------------|------------|-------------|-------------|--------------|-------------|
| Motor                        | Static     | Dynamic     | Total       | FS (holding) | FS (curve)  |
| T0                           | 0          | 36.0690827  | 36.0690827  | 4.491381202  | 4.05471914  |
| T1                           | 167.195754 | 36.0690827  | 203.2648367 | 1.593979585  | 1.106930267 |
| T2                           | 68.52285   | 12.12007253 | 80.64292253 | 2.008855767  | 1.813550345 |
| T3                           | 14.95044   | 1.518964704 | 16.4694047  | 1.420816382  | -           |

The system runs on 12 V and 6 A and is powered by the bench top power supply available in the Mechatronics Lab. An XT-30 cable serves as the connector between from the power supply and the PCB. All peripheral devices connect to the board using jst or dupont connectors.

### Electrical Design

- PCB
- STM32H7
- TMC429 Motion Controller
- TCS34725 Color Sensor
- Servo Gripper
- overdesigned - power, # of motors, USB, limit switches

The system runs off a custom-designed PCB. The following are key components:
- Power distribution:
    - 5 V switching regulator takes in up to 42 V DC power and outputs 5 V. The 5 V rail powers the servo motor.
    - 
- STM32H7 microcontroller
    This MCU was selected for its advanced features compared to the STM32FH with marginal cost increase. It has a double precision floating-point unit and a higher clock speed.
- The

PCB was designed in Fusion 360 Electronics. The board was manufacutred and assembled by JLCPCB.

## Software Architecture

- drivers
- open-source component libraries
- wrappers / HALs
- main/fsm?

\image html FSM.png "Finite State Machine for Robotic Arm Operation"

The Finite State Machine depicted above was implemented to structure and organize timing of the full functionality of the arm. The following states make up the FSM:

### State 1: Init
The init state runs once at startup and immediately commands the arm to move to its predefined initial pose by calling execute_trajectory with the initial_pose trajectory defined in maneuvers.c. This ensures the arm is in a known, safe configuration before the operator begins manual control. Once the trajectory completes, the FSM unconditionally transitions to the joystick state.

### State 2: Joystick
This state is the operator's primary interface for positioning the arm over a target block. On every run of main.c in this state, Joystick_Read is called from joystick.c, which uses HAL_ADC_ConfigChannel to toggle ADC1 between channel 16 (Y axis) and channel 17 (X axis), reading each in sequence via HAL_ADC_Start and HAL_ADC_PollForConversion. The raw 12-bit ADC values are filtered to eliminate noise and produce directional outputs of 0, 1, or 2 for each axis (no move, negative move, positive move). Back in fsm.c, these directional values drive move_to_pos calls on Motor 1 and Motor 2 from motion.c, with the new target position limited within predefined joint limits defined by to avoid cable tangling and grounding of the arm. The button is read via HAL_GPIO_ReadPin on the SEL pin, and a button press transitions the FSM to the get color state.

### State 3: Get Color
This state polls the TCS34725 color sensor until a valid color reading is returned. FSM_Update calls COLOR_SENSOR_Read in color_sensor.c. This function uses the tcs34725_read_rgbc vendor driver (driver_tcs34725.c) over I2C. The raw RGBC values are normalized by dividing by the clear channel, then passed to the classify function which computes the HSV hue angle and maps it to a ColorResult_t enum value defined in color_sensor.h. If the clear channel is below MIN_CLEAR or the saturation ratio is below 0.15, COLOR_UNKNOWN is returned and the state loops, re-reading until a valid result is obtained. Once reading of red, yellow, green, or blue is detected, it is stored in the detected_color variable in fsm.c and the FSM transitions to the move block state.

### State 4: Move Block
This state executes the full pick-and-place trajectory corresponding to the detected color. At entry, the current joint positions of Motor 1 and Motor 2 are captured via get_current_pos and written into the first two waypoints of the selected trajectory, ensuring the arm starts its motion from wherever the operator left it rather than transitioning to a fixed start position. Based on detected_color, one of four trajectories — red_trajectory, yellow_trajectory, green_trajectory, or blue_trajectory — defined in maneuvers.c is passed to execute_trajectory, which steps through each waypoint sequentially, commanding all four motors via move_to_pos and blocking until all motors reaches their target within a radian tolerance before advancing. Gripper open and close commands are embedded directly in the waypoint sequence via the servo field, triggering Servo_Open or Servo_Close from servo.c at the appropriate steps. Once the final waypoint is reached and the arm returns home, the FSM transitions back to the init state to reset for the next block.

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
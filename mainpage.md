# Project Overview
Authors: Ethan Dickson, Andrew Jones, & Sara Chamness

## Introduction

This website documents the design of a 4.5 DOF robotic arm. Our team created this robot for our term project in ME 507 at Cal Poly, San Luis Obispo.

The robot is a semi autonomous color-sorting device. The arm consists of four joints mobilized by stepper motors and a gripper controlled by a servo motor. It was designed to pick up small foam blocks and sort them by color. To operate the device, the user manipulates a joystick to direct the robot to the block, then presses the select button on the joystick. A color sensor mounted on the end effector reads the color and the robot autonomously brings the block to a predetermined location based on its color.

The system is built around a custom-designed PCB featuring an STM32H7 microcontroller. The project was developed using the STM32 ecosystem, and the firmware was implemented in C. The project utilizes open-source hardware drivers for interfacing with more complex electronics components, as well as the STM HAL library to work simplify communication with the microcontroller.

\image html RobotWithBlocks.png "Top View of Robot with Blocks" width=50%

## Hardware



### Mechanical Design

\image html CAD-ISO.png "Isometric View of CAD"

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

The system runs off a custom PCB designed in Fusion 360 Electronics and manufacutred by JLCPCB. All SMD components were assembled by JLC, and PTH components were soldered by hand. Key features and components are as follows:
- Power distribution
    The board was designed to handle 24 V. We discovered during implementation that 12 V was sufficient, but the power distribution includes the following: 
    - 5 V switching regulator takes in up to 42 V DC power and outputs 5 V. The 5 V rail powers the servo motor.
    - LDO takes power from the 5 V rail and converts it to 3.3 V. VDD for the MCU is 3.3, and most pheripheral components run on 3.3 V power and logic.
- STM32H7 microcontroller
    This MCU was selected over the STM32FH for its advanced features with marginal cost increase. It has a double precision floating-point unit and a higher clock speed.
- TMC429 Motion Controller
    This integrated circuit sits between the MCU and the motor drivers. Its sole purpose is motion control, so it smooths out motor motion by sending pulses to the motor drivers at the correct times. In this project, it is operated in ramp mode. It receives a position command from the MCU over SPI, plans acceleration and deceleration accordingly, and sends pulses to the motor drivers.
- Stepper Drivers
    Stepper motor drivers were used to control the four stepper motors. The drivers were configured for microsteping for higher resoltion motion. Enabling microstepping reduces the motor torque, but this application does not require the full motor torque. The stepper motors have two phases, and timed current flow to the phases controls the rotor motion. The stepper drivers receive pulses from the motion planners and send them to the motors.
- test LEDs
    The board has several test LEDs to help with debugging. The power supply rail, 5 V rail, and VDD rail each have an indicator LED. Each motor driver has two LEDs for visual indiation of when the two fault settings are triggered.
- Crystal Oscillator
    The board uses an 8 MHz crystal oscillator for more precise timing
- Flashing and Debugging
    Firmware development was done with an ST-Link, connected to the PCB using SWD.
- Pinouts
    The PCB has pin headers to connect all peripheral devices.

The PCB was over designed and allows several features that went unused (limit switches, a USB mini connector, extra motor drivers). These features allow for further development and increasing complexity in future iterations.

The system uses a TCS34725 Color Sensor breakout board for block color detection. The color sensor communicates with the MCU over I2C. A 2 DOF joystick is held in the user's hand and wired to the MCU. The joystick provides an analog voltage to the MCU, which is read by an ADC on the MCU. Lastly, the servo motor that moves the gripper receives PWM signaling from the MCU. Varying the pulse width changes the servo position.


## Software Architecture

The firmware is written in C. Hardware drivers from open-source resources were utilized for interfacing with the motion planning chip and the color sensor. Custom drivers, wrappers, and functions were written to simplify use of each component. Execution of tasks and functions and task logic is implemented via a finitie state machine. The state machine is called from main. The state machine is described by the state-transition diagram below:

photo???? Description of robot states??

### Motion Control

Motion control is done by specifying a target postion, rather than kinematics or inverse kinematics. The stepper motors and motion-planning chips made this possible and greatly reduce the computational complexity of the robot motion. For the autonomous motion, each desired path is defined by a trajector struct. The struct contains a series of waypoints that map out the path. When the execute_trajectory function is run, the MCU and motion planning chips sequentially feed the positions of each motor at each waypoint to the motor drivers. This design made implementing autonomous motion very simple, since new trajectories could be easily created by defining a series of waypoitns and the angles/motor positions at each waypoint.


### Joystick Readings

The joystick drives two degrees of the robot motion. Horizonal movement of the joystick rotates the base (motor 1) and vertical movement of the joystick rotates motor ?? The joystick readings are filtered so that the joystick must be more than halfway to the maximum or minimum position in any direction in order for the reading to be accepted and used to direct motion. This filters out noise in the readings and ensures only deliberate manipulation of the joystick moves the motors. The joystick can be used to move both motors 1 and ?? at once if it is moved at an angle.

### Color Detection

The MCU receives RBG and clear readings from the color sensor over I2C. The RBG results are normalized against the clear values. The code uses the normalized values to compute a hue angle between 0 and 360 degrees. The angles are mapped into six categories and used to determine the object color: red, orange, yellow, green, blue, and purple. We found that red, yellow, green, and blue objects were more consistenly classifiable, and so only those four colors were shown during the demo.

## Results

video!!!

## Challenges
- SPI bugs (expecting/sending different bit sizes, bug in the driver, reset pin high)
- joystick interrupts not working
- SWD finicky
- mechanical mounting of color sensor/gripping sequence

## Future Improvements

Future improvements to the design could include adding limit switches to the motors so that the home postition does not have to be set manually. The servo grip on the blocks could also be improved. More joysticks could be added and kinematics could be integrated to allow for more advanced path planning. Object detection could also be integrated to make the system fully autonomous.

## References

1. TMC429 Datasheet
2. STM32H7 Reference Manual
3. TCS34725 Datasheet
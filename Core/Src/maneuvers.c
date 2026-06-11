#include "maneuvers.h"
#include "motion.h"
#include <math.h>
#include <stdbool.h>
#include "main.h"
#include "servo.h"
#include "stm32h7xx_hal_def.h"

#define NUM_MOTORS 4 //Number of motors in the system

//Robot dimensions
#define L0  0.0
#define L1  1.0
#define L2  1.0
#define L3  1.0

///Define trajectory arrays
trajectory_t test_trajectory1 = {
    .waypoints = {
        {NA, 0.0, 0.0, -M_PI / 4, M_PI / 4},
        {NA, 0.0, 0.0, -M_PI / 2, M_PI / 2}, //Take 2 steps to get end effector close and parallel to grouns
        {NA, -M_PI / 4, 0.0, -M_PI / 2, M_PI / 2}, //rotate
        {NA, -M_PI / 4, 0.0, -M_PI / 4, M_PI / 9}, //Reach out to grab block
        {Close, -M_PI / 4, 0.0, -M_PI / 4, M_PI / 4}, //lift block
        {NA, -M_PI / 4, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //lift block waaay up
        {NA, -M_PI / 2, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //Rotate to new position above bin
        {NA, -M_PI / 2, M_PI/3, -M_PI / 3, 0}, //Extend over bin
        {Open, 0.0, 2*M_PI/3, -M_PI / 3, 0}, //Rotate Home
        {NA, 0.0, 0.0, 0.0, 0.0} //Return to home position
    },
    .num_waypoints = 10
};

static double tolerance = 0.2; //tolerance (in radians) for checking if at position
static int current_waypoint_index = 0; //index of current waypoint in trajectory
//Bools to track if each motor is at its target position for the current waypoint
static bool M1_at_target = false;
static bool M2_at_target = false;
static bool M3_at_target = false;
static bool M4_at_target = false;

//Function to check if all motors are at their target positions for the current waypoint
static bool q_at_waypoint(motor_config_t *motorConfigs, trajectory_t *trajectory) {
    //If each motor wasn't previously at its target position, check if it is now.
    if (!M1_at_target) {
        M1_at_target = fabs(get_current_pos(&motorConfigs[0]) - trajectory->waypoints[current_waypoint_index].joint1) < tolerance;
    }
    if (!M2_at_target) {
        M2_at_target = fabs(get_current_pos(&motorConfigs[1]) - trajectory->waypoints[current_waypoint_index].joint2) < tolerance;
    }
    if (!M3_at_target) {
        M3_at_target = fabs(get_current_pos(&motorConfigs[2]) - trajectory->waypoints[current_waypoint_index].joint3) < tolerance;
    }
    if (!M4_at_target) {
        M4_at_target = fabs(get_current_pos(&motorConfigs[3]) - trajectory->waypoints[current_waypoint_index].joint4) < tolerance;
    }
    if (M1_at_target && M2_at_target && M3_at_target && M4_at_target) {
        return true;
    }
    return false;
}

static bool go_to_next_waypoint(motor_config_t *motorConfigs, trajectory_t *trajectory) {
    //Reset bools for next waypoint
    M1_at_target = false;
    M2_at_target = false;
    M3_at_target = false;
    M4_at_target = false;
    current_waypoint_index++; //Bump up waypoint index to move to the next waypoint in the trajectory
    if (trajectory->waypoints[current_waypoint_index].servo == Open) {
        HAL_Delay(2000); //Delay to give time for servo to open before moving the arm
        Servo_Open();
        HAL_Delay(5000); //Delay to give time for servo to open before moving the arm
    }
    else if (trajectory->waypoints[current_waypoint_index].servo == Close) {
        HAL_Delay(2000); //Delay to give time for servo to open before moving the arm
        Servo_Close();
        HAL_Delay(5000); //Delay to give time for servo to close before moving the arm
    }
    if (current_waypoint_index >= trajectory->num_waypoints) {
        return false; //If we've reached the end of the trajectory, return false to indicate we are done
    }
    //Send next waypoint positions
    move_to_pos(&motorConfigs[0], trajectory->waypoints[current_waypoint_index].joint1);
    move_to_pos(&motorConfigs[1], trajectory->waypoints[current_waypoint_index].joint2);
    move_to_pos(&motorConfigs[2], trajectory->waypoints[current_waypoint_index].joint3);
    move_to_pos(&motorConfigs[3], trajectory->waypoints[current_waypoint_index].joint4);
    return true;
}

void execute_trajectory(motor_config_t *motorConfigs, trajectory_t *trajectory) {
    current_waypoint_index = 0; //Start at the first waypoint
    //Send first waypoint positions
    move_to_pos(&motorConfigs[0], trajectory->waypoints[current_waypoint_index].joint1);
    move_to_pos(&motorConfigs[1], trajectory->waypoints[current_waypoint_index].joint2);
    move_to_pos(&motorConfigs[2], trajectory->waypoints[current_waypoint_index].joint3);
    move_to_pos(&motorConfigs[3], trajectory->waypoints[current_waypoint_index].joint4);
    //Wait until we are at the current waypoint before moving to the next one
    while (true) {  
        while (!q_at_waypoint(motorConfigs, trajectory)) {
            //Do nothing, just wait
            HAL_Delay(1);
        }
        if (!go_to_next_waypoint(motorConfigs, trajectory)) {
            return;
        }
    }
}

/*
 * Computes end-effector position from joint angles.
 *
 * angles: pointer to theta0-theta3 joint angles, in radians
 * pose:   pointer to output x, y, z position
 */
static void ComputeEndEffectorPosition(const JointAngles_t *angles,
                                      EndEffectorPose_t *pose)
{
    
    double theta0 = angles->theta0;
    double theta1 = angles->theta1;
    double theta2 = angles->theta2;
    double theta3 = angles->theta3;

    double theta12  = theta1 + theta2;
    double theta123 = theta12 + theta3;

    double s0 = sin(theta0);
    double c0 = cos(theta0);


    double s1 = sin(theta1);
    double c1 = cos(theta1);
    double s12  = sin(theta12);
    double c12  = cos(theta12);
    double s123 = sin(theta123);
    double c123 = cos(theta123);

    double radial =
        (L1 * c1) +
        (L2 * c12) +
        (L3 * c123);

    pose->x = -s0 * radial;
    pose->y =  c0 * radial;
    pose->z =
        L0 +
        (L1 * s1) +
        (L2 * s12) +
        (L3 * s123);
}

void GetClawPose(motor_config_t *motorConfigs, EndEffectorPose_t *pose) {
    JointAngles_t angles = {
        .theta0 = get_current_pos(&motorConfigs[0]),
        .theta1 = get_current_pos(&motorConfigs[1]),
        .theta2 = get_current_pos(&motorConfigs[2]),
        .theta3 = get_current_pos(&motorConfigs[3])
    };
    ComputeEndEffectorPosition(&angles, pose);
}



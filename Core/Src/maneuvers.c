#include "maneuvers.h"
#include "motion.h"
#include <stdbool.h>
#include "main.h"

#define NUM_MOTORS 4 //Number of motors in the system

///Define trajectory arrays
trajectory_t test_trajectory1 = {
    .waypoints = {
        {false, 0.0, 0.0, -M_PI / 4, M_PI / 4},
        {false, 0.0, 0.0, -M_PI / 2, M_PI / 2}, //Take 2 steps to get end effector close and parallel to grouns
        {false, -M_PI / 4, 0.0, -M_PI / 2, M_PI / 2}, //rotate
        {false, -M_PI / 4, 0.0, -M_PI / 4, M_PI / 9}, //Reach out to grab block
        {true, -M_PI / 4, 0.0, -M_PI / 4, M_PI / 4}, //lift block
        {false, -M_PI / 4, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //lift block waaay up
        {false, -M_PI / 2, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //Rotate to new position above bin
        {false, -M_PI / 2, M_PI/3, -M_PI / 3, 0}, //Extend over bin
        {true, 0.0, 2*M_PI/3, -M_PI / 3, 0}, //Rotate Home
        {false, 0.0, 0.0, 0.0, 0.0} //Return to home position
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
    if (trajectory->waypoints[current_waypoint_index].delay) {
        HAL_Delay(5000); //If this waypoint has the delay flag, wait 5s
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

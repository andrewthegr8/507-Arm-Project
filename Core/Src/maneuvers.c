/**
 * @file maneuvers.c
 * @brief High-level motion/trajectory utilities for the robot arm.
 *
 * This module exposes predefined trajectories and routines to execute
 * them on the arm's motors. It also provides kinematic helpers to
 * compute the end-effector pose from joint angles.
 */

#include "maneuvers.h"
#include "motion.h"
#include <math.h>
#include <stdbool.h>
#include "main.h"
#include "servo.h"
#include "stm32h7xx_hal_def.h"

/** @brief Number of actuated motors in the system. */
#define NUM_MOTORS 4

/* Robot link dimensions (mm). */
/** @brief Base offset (along Z). */
#define L0  153
/** @brief Length of first link. */
#define L1  121
/** @brief Length of second link. */
#define L2  121
/** @brief Length of third link. */
#define L3  150

/** @name Predefined trajectories
 *  @brief Collection of commonly used waypoint sequences for testing and
 *  gameplay (bin placement, initial poses, demo routines, etc.).
 *  @{ */
/** @brief Example test trajectory with multiple waypoints used for development. */
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

/** @brief Initial pose used to start joystick control. */
trajectory_t initial_pose = {
    .waypoints = {
        {NA, 0.0, M_PI/2, 0.0, -M_PI/2}, //Initial pose to start in B4 joystick control
    },
    .num_waypoints = 1
};
/** @brief Trajectory to place blocks in the red bin. */
trajectory_t red_trajectory = {  
    .waypoints = {
        {Close, 0.0, 0.0, M_PI / 4, M_PI / 4}, //lift block
        {NA, 0.0, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //lift block waaay up
        {NA, -M_PI/4, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //Rotate to new position above bin
        {NA, -M_PI/4, M_PI/3, -M_PI / 3, 0}, //Extend over bin
        {Open, -M_PI/4, M_PI/3, -M_PI / 3, 0}, //Rotate Home
    },
    .num_waypoints = 5
};
/** @brief Trajectory to place blocks in the yellow bin. */
trajectory_t yellow_trajectory = {
    .waypoints = {
        {Close, 0.0, 0.0, M_PI / 4, M_PI / 4}, //lift block
        {NA, 0.0, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //lift block waaay up
        {NA, -3*M_PI/4, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //Rotate to new position above bin
        {NA, -3*M_PI/4, M_PI/3, -M_PI / 3, 0}, //Extend over bin
        {Open, -3*M_PI/4, 2*M_PI/3, -M_PI / 3, 0}, //Rotate Home
    },
    .num_waypoints = 5
  };
/** @brief Trajectory to place blocks in the green bin. */
trajectory_t green_trajectory = {  
    .waypoints = {
        {Close, 0.0, 0.0, M_PI / 4, M_PI / 4}, //lift block
        {NA, 0.0, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //lift block waaay up
        {NA, 3*M_PI/4, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //Rotate to new position above bin
        {NA, 3*M_PI/4, M_PI/3, -M_PI / 3, 0}, //Extend over bin
        {Open, 3*M_PI/4, 2*M_PI/3, -M_PI / 3, 0}, //Rotate Home
    },
    .num_waypoints = 5
};
/** @brief Trajectory to place blocks in the blue bin. */
trajectory_t blue_trajectory = {  
    .waypoints = {
        {Close, 0.0, 0.0, M_PI / 4, M_PI / 4}, //lift block
        {NA, 0.0, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //lift block waaay up
        {NA, M_PI/4, 2*M_PI/3, -M_PI / 3, -M_PI / 2}, //Rotate to new position above bin
        {NA, M_PI/4, M_PI/3, -M_PI / 3, 0}, //Extend over bin
        {Open, M_PI/4, 2*M_PI/3, -M_PI / 3, 0}, //Rotate Home
    },
    .num_waypoints = 5
};

trajectory_t trajectory1 = {
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

/** @} */

/** @brief Tolerance (radians) used to decide whether a motor reached target. */
static double tolerance = 0.2;

/** @brief Index of the currently active waypoint within the current trajectory. */
static int current_waypoint_index = 0;

/** @brief Per-motor flags that indicate if the motor reached the current waypoint target. */
static bool M1_at_target = false;
static bool M2_at_target = false;
static bool M3_at_target = false;
static bool M4_at_target = false;

/**
 * @brief Check whether all motors are within tolerance of the current waypoint targets.
 *
 * This function updates the per-motor "_at_target" flags the first time the motor
 * is observed within the configured tolerance and then returns true once all motors
 * are marked as at-target.
 *
 * @param motorConfigs Pointer to an array of `motor_config_t` of length at least `NUM_MOTORS`.
 * @param trajectory Pointer to the active `trajectory_t` whose `waypoints` contain target positions.
 * @return true if all motors have reached their target positions for the current waypoint.
 */
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

/**
 * @brief Advance to the next waypoint and command motors and servos accordingly.
 *
 * This routine updates internal "at target" flags, increments the internal
 * waypoint index, issues servo open/close commands for the upcoming waypoint
 * (with modest delays to allow actuation), and commands each motor to move to
 * its next joint target.
 *
 * @param motorConfigs Pointer to an array of `motor_config_t` (length >= NUM_MOTORS).
 * @param trajectory Pointer to the trajectory being executed.
 * @return true if there is a next waypoint and motors were commanded; false if
 *         the trajectory has completed.
 */
static bool go_to_next_waypoint(motor_config_t *motorConfigs, trajectory_t *trajectory) {
    /* Reset per-motor reached flags for the upcoming waypoint. */
    M1_at_target = false;
    M2_at_target = false;
    M3_at_target = false;
    M4_at_target = false;
    current_waypoint_index++; /* Move to the next waypoint */

    /* Handle servo command associated with the new waypoint, if any. */
    if (trajectory->waypoints[current_waypoint_index].servo == Open) {
        HAL_Delay(1000);
        Servo_Open();
        HAL_Delay(5000);
    }
    else if (trajectory->waypoints[current_waypoint_index].servo == Close) {
        HAL_Delay(1000);
        Servo_Close();
        HAL_Delay(5000);
    }

    /* If we've advanced past the final waypoint, the trajectory is complete. */
    if (current_waypoint_index >= trajectory->num_waypoints) {
        return false;
    }

    /* Command each motor to the next waypoint joint angles. */
    move_to_pos(&motorConfigs[0], trajectory->waypoints[current_waypoint_index].joint1);
    move_to_pos(&motorConfigs[1], trajectory->waypoints[current_waypoint_index].joint2);
    move_to_pos(&motorConfigs[2], trajectory->waypoints[current_waypoint_index].joint3);
    move_to_pos(&motorConfigs[3], trajectory->waypoints[current_waypoint_index].joint4);
    return true;
}

/**
 * @brief Execute a trajectory from its first waypoint through completion.
 *
 * This function blocks until the entire trajectory has been executed. It
 * handles any initial servo command, sends the initial joint commands, then
 * repeatedly waits until the current waypoint is reached before advancing to
 * the next waypoint.
 *
 * @param motorConfigs Pointer to an array of `motor_config_t` (length >= NUM_MOTORS).
 * @param trajectory Pointer to the `trajectory_t` to execute.
 */
void execute_trajectory(motor_config_t *motorConfigs, trajectory_t *trajectory) {
    current_waypoint_index = 0; /* start at the first waypoint */

    /* Handle any servo command attached to the initial waypoint. */
    if (trajectory->waypoints[current_waypoint_index].servo == Open) {
        HAL_Delay(1000);
        Servo_Open();
        HAL_Delay(5000);
    }
    else if (trajectory->waypoints[current_waypoint_index].servo == Close) {
        HAL_Delay(1000);
        Servo_Close();
        HAL_Delay(5000);
    }

    /* Send first waypoint positions to all motors. */
    move_to_pos(&motorConfigs[0], trajectory->waypoints[current_waypoint_index].joint1);
    move_to_pos(&motorConfigs[1], trajectory->waypoints[current_waypoint_index].joint2);
    move_to_pos(&motorConfigs[2], trajectory->waypoints[current_waypoint_index].joint3);
    move_to_pos(&motorConfigs[3], trajectory->waypoints[current_waypoint_index].joint4);

    /* Wait until we are at the current waypoint before moving to the next one. */
    while (true) {
        while (!q_at_waypoint(motorConfigs, trajectory)) {
            HAL_Delay(1);
        }
        if (!go_to_next_waypoint(motorConfigs, trajectory)) {
            return; /* trajectory complete */
        }
    }
}

/**
 * @brief Compute end-effector Cartesian pose from joint angles.
 *
 * Performs forward kinematics for the 4-DOF arm. The input angles are in
 * radians and correspond to the revolute joints theta0..theta3. The computed
 * pose includes x, y, z coordinates and the radial projection `r`.
 *
 * @param angles Pointer to `JointAngles_t` containing `theta0`..`theta3` in radians.
 * @param pose Output pointer to `EndEffectorPose_t` that will be populated.
 * @note Link lengths L0..L3 are used from this translation unit.
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
    pose->r = radial;
}

/**
 * @brief Compute the current claw (end-effector) pose using live motor positions.
 *
 * This helper reads the current joint positions from `motorConfigs`, assembles
 * a `JointAngles_t`, and calls `ComputeEndEffectorPosition` to fill `pose`.
 *
 * @param motorConfigs Pointer to an array of `motor_config_t` (length >= NUM_MOTORS).
 * @param pose Output pointer to `EndEffectorPose_t` to receive the computed pose.
 */
void GetClawPose(motor_config_t *motorConfigs, EndEffectorPose_t *pose) {
    JointAngles_t angles = {
        .theta0 = get_current_pos(&motorConfigs[0]),
        .theta1 = get_current_pos(&motorConfigs[1]),
        .theta2 = get_current_pos(&motorConfigs[2]),
        .theta3 = get_current_pos(&motorConfigs[3])
    };
    ComputeEndEffectorPosition(&angles, pose);
}



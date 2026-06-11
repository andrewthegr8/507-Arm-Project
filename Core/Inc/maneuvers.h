#ifndef MANEUVERS_H
#define MANEUVERS_H
#include "motion.h"

/**
 * @defgroup MotionControl Motion Control
 * @brief Color sensor reading and classification.
 * @{
 */

//Type to specify servo command for a given waypoint
typedef enum {
    NA = 0,
    Open = 1,
    Close = 2
} servo_cmd_t;


//type to store joint angles for a given waypoint
typedef struct {
    servo_cmd_t servo; //Servo command for this waypoint
    double joint1;
    double joint2;
    double joint3;
    double joint4;
} waypoint_t;


//type to store a trajectory, which is an array of waypoints and the number of waypoints in the trajectory
typedef struct {
    waypoint_t waypoints[15]; //max number of waypoints in a trajectory
    int num_waypoints;
} trajectory_t;


/*
 * Joint angles must be in radians.
 */
typedef struct
{
    double theta0;
    double theta1;
    double theta2;
    double theta3;
} JointAngles_t;

typedef struct
{
    double x;
    double y;
    double z;
} EndEffectorPose_t;

extern trajectory_t test_trajectory1; //Make trajectories accesisble from main

void execute_trajectory(motor_config_t *motorConfigs, trajectory_t *trajectory);
void GetClawPose(motor_config_t *motorConfigs, EndEffectorPose_t *pose);

/** @} */

#endif
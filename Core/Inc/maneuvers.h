#ifndef MANEUVERS_H
#define MANEUVERS_H
#include "motion.h"


//type to store joint angles for a given waypoint
typedef struct {
    bool delay; //Whether to delay at this waypoint or not (used for waiting to close gripper after reaching block)
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

extern trajectory_t test_trajectory1; //Make trajectories accesisble from main

void execute_trajectory(motor_config_t *motorConfigs, trajectory_t *trajectory);

#endif



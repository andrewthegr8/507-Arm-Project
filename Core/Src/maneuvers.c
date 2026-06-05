#include "maneuvers.h"
#include "motion.h"
#include <stdbool.h>
#include "main.h"

#define NUM_MOTORS 4 //Number of motors in the system

///Define trajectory arrays
trajectory_t test_trajectory1 = {
    .waypoints = {
        {0.0, 0.0, 0.0, 0.0},
        {M_PI / 4, M_PI / 4, M_PI / 4, M_PI / 4},
        {M_PI / 2, M_PI / 2, M_PI / 2, M_PI / 2},
        {3 * M_PI / 4, 3 * M_PI / 4, 3 * M_PI / 4, 3 * M_PI / 4},
        {M_PI, M_PI, M_PI, M_PI}
    },
    .num_waypoints = 5
};

static double tolerance = 0.1; //tolerance for checking if at position
static int current_waypoint_index = 0; //index of current waypoint in trajectory

//Function to execute a trajectory

bool is_at_waypoint(motor_config_t *motorConfig, trajectory_t trajectory) {
    //Make life easier and give each motor a bool to check if it's at target position
    bool M1_at_target = false;
    bool M2_at_target = false;
    bool M3_at_target = false;
    bool M4_at_target = false;
    for (int i = 0; i < NUM_MOTORS; i++) {
        if 
        

        double target_pos = trajectory.waypoints[current_waypoint_index].joint1; //get target position for current motor
    double current_pos = get_current_pos(motorConfig);
    return fabs(current_pos - target_pos) < tolerance;
}

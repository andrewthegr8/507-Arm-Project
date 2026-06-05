

//type to store joint angles for a given waypoint
typedef struct {
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


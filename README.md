# Infiltration!

### Problem Statement

A prototype (version 0.0.7) "Bond" autonomous infiltration robot finds
itself activated at a random (and unknown) location in one of three
(also random and unknown to the robot) drop zones. It needs to make its
way to a target zone, undetected.

To reach the target zone, Bond must navigate a number of large
obstacles. It has a rough, but inexact, map of these obstacles. Bond's
primary sensor measures the range to any solid matter in a 60 degree arc
in front of the robot, allowing it to observe these obstacles.

In addition to the stationary obstacles, a number of quadcopter guards
slowly patrol the area. If they spot Bond, it is captured and fails.
Their paths are unknown to Bond, but are at least "smooth" (no sudden
sharp turns or accelerations). Their motion and sight are not restricted
by obstacles.

Fortunately, this was anticipated and Bond has been equipped with a
special long range sensor that can detect range and direction to
electric readings regardless of intervening material (e.g. the
obstacles).  The guards can only detect Bond at much shorter range, so
it should be able to use this sensor to avoid them.

### Approach

I will use a particle filter to perform the initial rough localization,
narrowing the robot's position to one of the drop zones. Once that
localization is completed, the localization strategy will switch to
SLAM.

I will compare the measurements of the obstacles with the believed
positions of the robot and obstacles to attempt to determine distance
from the obstacle to feed to SLAM. This will allow for more precise
localization of both the robot and the stationary obstacles.

I will use a Kalman filter to track the guards' position and direction.
Not knowing details of their paths prohibits long term prediction with
high confidence, but a belief about likelihood that a guard will be at a
specific location at a specific time can be constructed.

Combining belief about obstacle locations and guard locations gives an
avoidance map in which planning can be executed. I will use A* search to
maintain a maximal distance from the guards while navigating around
obstacles to reach the target zone.

### Metrics

Primary performance metrics will be:

 * Execution time required per simulation time step.

 * Error in believed position and heading vs. true position and heading
   over time. Expected to decrease with progression of the simulation.

 * Error in correlation between believed obstacle locations and true
   obstacle locations, over time. Expected to decrease with progression
   of the simulation.

 * Ability to avoid detection, measured as simulation time steps elapsed
   at time of detection.

 * Ability to progress toward the target zone, measured as distance from
   target at time of detection.

 * Ability to combine progress and avoidance into efficiency. Measured
   as the ratio of distance to target vs. simulation time steps elapsed
   at time of detection.

The progress metric has a (minimal) cap when the target zone is
successfully reached. But even when reaching the target, performance can
still be improved by increasing efficiency.

### Data Set

I have constructed the true map, guard patrol routines, noisy map
generation, and noisy sensor routines. They are available at this github
repo. All the robot's information and sensory input will be derived from
these routines.

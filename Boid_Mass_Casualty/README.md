# Boid Mass Casualty Simulation

This is a project I made up in prepration for a job at in ARA's HEART directorate.

I'm going to start with a simple boid simulation where boids are attracted to the local center of mass, repealed by their neighbors, and align their velocities. They will avoid the walls of the simulation space.

Once I have a simulation of boids, I'm going to introduce a mass casualty event of a firework going off in the flock. Uninjured and flying injured boid will roost. EMT boids will take flight and recover critically injured boids returning them to a roost in triage. Critcally injured boids will not fall to the ground to keep things slightly more interesting.

## Compile directions for g++

g++ Sim.cpp Boid.cpp -O3

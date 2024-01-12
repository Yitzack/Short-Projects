# Detination Simulation

I'm going to try to make a fluid simulation of an amount of TNT going off in a small concrete box.  The air is going to be a fluid simulation with Euler Equations for compressible fluid. The concrete is going to be masses on springs that will break of they get too long. Accuracy is not gurenteed. It is likely to be quite hilarous when animated.

The simulation volume is going to be a cube 4 meters on a side. The concrete box is going to be 3 meters on all interior edges and 15 cm thick and resting on the ground. I'm going to put a simulated 1 kg TNT detenation at the center. And then the simulation will roll.

## Compile directions for g++

g++ Sim.cpp Mass_Spring.cpp Air.cpp Voxel.cpp -O3

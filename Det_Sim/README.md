# Detination Simulation

I was going to try to make a fluid simulation of an amount of TNT going off in a small concrete box.  Turns out that doing air is hard without an outside library. I'm going to substitute an analytic approximation of a blast wave from [literature](https://academic.oup.com/mnras/article/424/4/2522/1055350). This blast wave is technically intended for super nova. But I didn't see an assumption in the paper indicating that it can only be used in for astronomical phenomena. So I'll appropriate it into this simulation.

The concrete is going to be masses on springs that will break of they get too long. Accuracy is not guaranteed. It is likely to be quite hilarious when animated.

The concrete box is going to be 3 meters on all interior edges and 15 cm thick and resting on the ground. I'm going to put a simulated 1 kg TNT detention at the center. And then the simulation will roll.

There is so much missing. There is no gravity or ambient air pressure because I couldn't create the bunker in equilibrium. The thing would convert all of that potential energy into kinetic energy and explode without the blastwave. It doesn't have collision detection with anything other than the ground, not that I gave it much time to sink through the ground. I started the simulation of the bunker at 165 microseconds. The blastwave is shreading the bunker by 173 microseconds. I was hoping for chunks of concrete, but every voxel of concrete is a surface voxel by 288 microseconds suggesting that none is connected to its neighbor. It would appear that the speed of sound in concete is greater than the speed of sound in air as the roof distingrates before the blastwave reaches it.

## Compile directions for g++

g++ Sim.cpp Mass_Spring.cpp Vector3.cpp -O3 -fopenmp -o Sim  
g++ Replot.cpp -O3 -o Replot

## Run directions

Requires gnuplot to be installed. Otherwise, a gnuplot script will be written to the terminal with nowhere to go.

./Sim | gnuplot  
./Replot | gnuplot

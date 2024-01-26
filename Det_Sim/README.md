# Detination Simulation

I was going to try to make a fluid simulation of an amount of TNT going off in a small concrete box.  Turns out that doing air is hard without an outside library. I'm going to substitute an analytic approximation of a blast wave from [literature](https://academic.oup.com/mnras/article/424/4/2522/1055350). This blast wave is technically intended for super nova. But I didn't see an assumption in the paper indicating that it can only be used in for astronomical phenomena. So I'll appropriate it into this simulation.

The concrete is going to be masses on springs that will break of they get too long. Accuracy is not guaranteed. It is likely to be quite hilarious when animated.

The concrete box is going to be 3 meters on all interior edges and 15 cm thick and resting on the ground. I'm going to put a simulated 1 kg TNT detention at the center. And then the simulation will roll.

## Compile directions for g++

g++ Sim.cpp Mass_Spring.cpp Vector3.cpp -O3 -fopenmp -o Sim 
g++ Replot.cpp -O3 -o Replot

## Run directions

Requires gnuplot to be installed. Otherwise, a gnuplot script will be written to the terminal with nowhere to go.

./Sim | gnuplot 
./Replot | gnuplot

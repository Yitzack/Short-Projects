# Mandelbrot Set

Displays the Mandelbrot set. I'll probably add controls for navigating and zooming later. I'll be happy if I can get the set to materialize at first.

## Compile directions for g++

Install SFML first. If you're on an approiate version of Linux, use ```sudo apt-get install libssfml-dev```  
g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system -fopenmp -p

Install python3-tk package first.
```python3 main.py``` will execute the script if the dependency has been met. Python is fast than C++, but that's because there's barrier block that prevents all threads from going at max speed but it also prevents data races that result in segfaults.

## Future version considerations

I keep seeing job postings for Java and Python. Once I know how to do this in C++, I'll rewrite it in those, starting with Python as I know I have that installed. I have Java installed for Minecraft, but I've never written anything for it.

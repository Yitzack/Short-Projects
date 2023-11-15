# Mandelbrot Set

Displays the Mandelbrot set.

## Compile directions

Install SFML first. If you're on an approiate version of Linux, use ```sudo apt-get install libssfml-dev```  
```g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system -fopenmp -p```

Install tkinter (aka python3-tk) package first.  
```python3 main.py``` will execute the script if the dependency has been met.

```javac Complex.java```  
```javac Pixel.java```  
```javac Mandelbrot.java```  
Java is stinking fast on a single core. There is a repaint reqeuest for every pass through the loop, but its fast enough that I can't tell. It also throws a bunch of exceptions. But because it produces the expected image, I'm not going to do anything about it.

## Other notes

The C++ can zoom in and out. Left mouse button zooms in at the click point. Right mouse button zooms out.

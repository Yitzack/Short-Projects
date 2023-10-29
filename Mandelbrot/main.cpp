#include<complex>
#include<iostream>
#include<SFML/Graphics.hpp>
using namespace std;
using namespace sf;

inline double Map(double x1, double x2, double y1, double y2, double x)	//returns the y for an input x given that x is between x1 and x2 and y is between y1 and y2
{
	return((y1-y2)*(x-x1)/(x1-x2)+y1);
}

int Mandelbrot(complex<double> c)
{
	complex<double> z(0,0);
	int i = 0;

	while(i < 256 && abs(z) < 2)
	{
		z = pow(z,2)+c;
		i++;
	}

	return(i);
}

int main()
{
	RenderWindow window(VideoMode(640, 480), "Mandelbrot");
	vector<Vertex> points;
	double x = 0;
	double y = 0;
	int iterations;
	int gray_level;
	int max = 256;

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				cout << max << endl;
				window.close();
			}
		}

		iterations = Mandelbrot(complex<double>(Map(0,640,-2,1,x),Map(0,480,-1.5,1.5,y)));
		if(iterations > max)
			max = iterations;
		gray_level = Map(0,256,0,max,iterations);
		points.push_back(Vertex(Vector2f(x,y), Color(gray_level,gray_level,gray_level)));

		window.clear();
		window.draw(&points[0], points.size(), Points);
		window.display();

		x++;
		if(x > 640)
		{
			x = 0;
			y++;
			if(y > 480)
				y = 0;
		}
	}

	return(0);
}

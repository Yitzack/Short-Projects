#include<complex>
#include<iostream>
#include<omp.h>
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
	vector<Vertex>* points;
	const int X_MAX = 640;
	const int Y_MAX = 480;
	RenderWindow window(VideoMode(X_MAX, Y_MAX), "Mandelbrot");//, Style::Fullscreen);
	vector<Vertex> All_Points;
	int max = 256;
	int threads;
	bool Calc_Pixels = true;
	//omp_set_num_threads(2);

	#pragma omp parallel
	{
		#pragma omp master
		{
			threads = omp_get_num_threads();
			points = new vector<Vertex>[threads];
		}
		#pragma omp barrier
	}

	while(window.isOpen())
	{
		Event event;

		if(Calc_Pixels)
		{
			#pragma omp for
			for(int x = 0; x < X_MAX; x++)
			{
				for(int y = 0; y < Y_MAX; y++)
				{
					int iterations;
					int gray_level;
					iterations = Mandelbrot(complex<double>(Map(0,X_MAX,-2,1,x),Map(0,Y_MAX,-1.5,1.5,y)));
					gray_level = Map(0,256,0,max,iterations);
					points[omp_get_thread_num()].push_back(Vertex(Vector2f(x,y), Color(gray_level,gray_level,gray_level)));
				}
			}

			int Total_Points = 0;
			All_Points.clear();
			for(int i = 0; i < threads; i++)
				Total_Points += points[i].size();
			All_Points.reserve(Total_Points);
			for(int i = 0; i < threads; i++)
				copy(points[i].begin(), points[i].end(), back_inserter(All_Points));

			window.clear();
			window.draw(&All_Points[0], All_Points.size(), Points);
			window.display();
			Calc_Pixels = false;
		}

		while(window.pollEvent(event))
		{
			if(event.type == Event::Closed)
				window.close();
		}
	}

	return(0);
}

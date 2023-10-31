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
	vector<Vertex> All_Points;
	double* x_start;
	double* x_end;
	double* y_start;
	double* y_end;
	int max = 256;
	int threads;
	//omp_set_num_threads(2);

	#pragma omp parallel
	{
		double x;
		double y;
		#pragma omp master
		{
			threads = omp_get_num_threads();
			x_start = new double[threads];
			x_end = new double[threads];
			y_start = new double[threads];
			y_end = new double[threads];
			points = new vector<Vertex>[threads];
			int y_rect = sqrt(threads);
			while(threads%y_rect != 0)
				y_rect--;
			int x_rect = threads/y_rect;
			for(int i = 0; i < threads; i++)
			{
				x_start[i] = float(X_MAX)/float(x_rect)*(i%x_rect);
				x_end[i] = float(X_MAX)/float(x_rect)*(i%x_rect+1);
				y_start[i] = float(Y_MAX)/float(y_rect)*((i/x_rect)%y_rect);
				y_end[i] = float(Y_MAX)/float(y_rect)*((i/x_rect)%y_rect+1);
			}
		}
		#pragma omp barrier
		x = x_start[omp_get_thread_num()];
		y = y_start[omp_get_thread_num()];
		points[omp_get_thread_num()].reserve(X_MAX*Y_MAX/threads+1);

		int iterations;
		int gray_level;

		for(x = x_start[omp_get_thread_num()]; x < x_end[omp_get_thread_num()]; x++)
		{
			for(y = y_start[omp_get_thread_num()]; y < y_end[omp_get_thread_num()]; y++)
			{
				iterations = Mandelbrot(complex<double>(Map(0,X_MAX,-2,1,x),Map(0,Y_MAX,-1.5,1.5,y)));
				if(iterations > max)
					max = iterations;
				gray_level = Map(0,256,0,max,iterations);
				points[omp_get_thread_num()].push_back(Vertex(Vector2f(x,y), Color(gray_level,gray_level,gray_level)));
			}
		}
		#pragma omp barrier
	}

	int Total_Points = 0;
	All_Points.clear();

	for(int i = 0; i < threads; i++)
		Total_Points += points[i].size();
	All_Points.reserve(Total_Points);

	for(int i = 0; i < threads; i++)
		copy(points[i].begin(), points[i].end(), back_inserter(All_Points));
	RenderWindow window(VideoMode(X_MAX, Y_MAX), "Mandelbrot");//, Style::Fullscreen);
	window.clear();
	window.draw(&All_Points[0], All_Points.size(), Points);
	window.display();

	while(window.isOpen())
	{
		Event event;
		while(window.pollEvent(event))
		{
			if(event.type == Event::Closed)
				window.close();
		}
	}

	return(0);
}

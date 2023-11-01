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
	int Window_Width = 640;
	int Window_Height = 480;
	double X_Min = -2;
	double X_Max = 1;
	double Y_Min = -1.5;
	double Y_Max = 1.5;
	RenderWindow window(VideoMode(Window_Width, Window_Height), "Mandelbrot");//, Style::Fullscreen);
	vector<Vertex> All_Points;
	int max = 256;
	int threads;
	bool Calc_Pixels = true;
	//omp_set_num_threads(1);

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
			Vector2u WindowSize = window.getSize();
			Window_Width = WindowSize.x;
			Window_Height = WindowSize.y;

			#pragma omp for
			for(int x = 0; x < Window_Width; x++)
			{
				for(int y = 0; y < Window_Height; y++)
				{
					int iterations;
					int gray_level;
					iterations = Mandelbrot(complex<double>(Map(0,Window_Width,X_Min,X_Max,x),Map(0,Window_Height,Y_Min,Y_Max,y)));
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
			{
				copy(points[i].begin(), points[i].end(), back_inserter(All_Points));
				points[i].clear();
			}

			window.clear();
			window.draw(&All_Points[0], All_Points.size(), Points);
			window.display();

			if(Window_Width != WindowSize.x || Window_Height != WindowSize.y)
			{
				FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			        window.setView(View(visibleArea));
				Calc_Pixels = true;
			}
			else
				Calc_Pixels = false;
		}

		while(window.pollEvent(event) && !Calc_Pixels)
		{
			switch(event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::MouseButtonPressed:
				if(event.mouseButton.button == Mouse::Button::Right)	//Zoom Out
				{
					double X_Width = X_Max-X_Min;
					double Y_Width = Y_Max-Y_Min;
					X_Min -= X_Width;
					X_Max += X_Width;
					Y_Min -= Y_Width;
					Y_Max += Y_Width;
					Calc_Pixels = true;
				}
				else if(event.mouseButton.button == Mouse::Button::Left)	//Zoom In
				{
					double X_Width = (X_Max-X_Min)/4.;
					double Y_Width = (Y_Max-Y_Min)/4.;
					double Click_X = Map(0,Window_Width,X_Min,X_Max,event.mouseButton.x);
					double Click_Y = Map(0,Window_Height,Y_Min,Y_Max,event.mouseButton.y);
					X_Min = Click_X-X_Width;
					X_Max = Click_X+X_Width;
					Y_Min = Click_Y-Y_Width;
					Y_Max = Click_Y+Y_Width;
					Calc_Pixels = true;
				}
				break;
			case Event::Resized:
				FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			        window.setView(View(visibleArea));
				Calc_Pixels = true;
				break;
			}
		}
	}

	return(0);
}

















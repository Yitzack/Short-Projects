#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

int main()
{
	RenderWindow window(VideoMode(640, 480), "Mandelbrot");
	vector<Vertex> points;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(&points[0], points.size(), Points);
		window.display();
	}

	return(0);
}

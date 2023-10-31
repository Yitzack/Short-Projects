import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;

public class Mandelbrot extends JPanel
{
	private ArrayList<Pixel> points;

	public Mandelbrot()
	{
		points = new ArrayList<>();
	}

	public void addPoint(int x, int y, int grey)
	{
		points.add(new Pixel(x, y, new Color(grey, grey, grey)));
//		repaint(); // Request the canvas to be repainted
	}

	public void Paint()
	{
		repaint(); // Request the canvas to be repainted
	}
	
	static double Map(double x1, double x2, double y1, double y2, double x)	//returns the y for an input x given that x is between x1 and x2 and y is between y1 and y2
	{
		return((y1-y2)*(x-x1)/(x1-x2)+y1);
	}

	static int Mandelbrot(Complex c)
	{
		Complex z = new Complex(0,0);
		int i = 0;

		while(i < 256 && z.abs() < 2)
		{
			z = z.square().add(c);
			i++;
		}

		return(i);
	}

	@Override
	protected void paintComponent(Graphics g)
	{
		super.paintComponent(g);
		for (Pixel point : points)
		{
			g.setColor(point.getColor()); // Set the color of the points
			g.fillRect(point.getX(), point.getY(), 1, 1); // Draw points as small rectangles
		}
	}

	public static void main(String[] args)
	{
		JFrame frame = new JFrame("Mandelbrot");
		Mandelbrot canvas = new Mandelbrot();
		frame.add(canvas);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(640, 480);
		frame.setVisible(true);

		// Example: Add points to the canvas
		for(int x = 0; x < 640; x++)
			for(int y = 0; y < 480; y++)
			{
				int iterations = Mandelbrot(new Complex(Map(0,640,-2,1,x),Map(0,480,-1.5,1.5,y)));
				canvas.addPoint(x, y, iterations%256);
			}
		
		canvas.Paint();
	}
}

import tkinter as tk
import cmath

# Create a function to draw a colored point on the canvas
def draw_point(canvas, x, y, color):
	canvas.create_oval(x, y, x + 1, y + 1, fill=color, outline=color)  # Draws a point as a small oval

def Map(x1, x2, y1, y2, x):	#returns the y for an input x given that x is between x1 and x2 and y is between y1 and y2
	return((y1-y2)*(x-x1)/(x1-x2)+y1)

def grey_scale(color):	#convert a number from 1-256 to a color code with 256 overflowing to 0
	color = color % 256;
	if(color < 16):
		return("#0"+str(hex(color))[2:3]+"0"+str(hex(color))[2:3]+"0"+str(hex(color))[2:3])
	else:
		return("#"+str(hex(color*256*256+color*256+color))[2:8])

def Mandelbrot(c):
	z = complex(0,0)
	i = 0
	while(i < 256 and abs(z) < 2):
		z = pow(z,2)+c
		i += 1
	return(i)

# Create a tkinter window
window = tk.Tk()
window.title("Mandelbrot")

# Create a canvas widget
canvas_width = 640
canvas_height = 480
canvas = tk.Canvas(window, width=canvas_width, height=canvas_height)
canvas.pack()

# Draw colored points on the canvas
for x in range(canvas_width):
	for y in range(canvas_height):
		iterations = Mandelbrot(complex(Map(0,canvas_width,-2,1,x),Map(0,canvas_height,-1.5,1.5,y)))
		color = grey_scale(iterations)
		draw_point(canvas, x, y, color)

# Start the tkinter main loop
window.mainloop()


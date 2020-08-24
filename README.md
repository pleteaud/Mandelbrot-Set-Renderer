# Mandelbrot-Set-Renderer

## Description:
	This is a Mandelbrot Fractal Renderer(using C++ and OpenGL). Rendering and coloring is done using the Escape Algorithm and Continous (smooth) coloring algorithm. 

## Libraries:
	GL3W 2-20-2019 (Last Commit) 
	GLM0 .9.9.5 (2019-04-01)
	STB Image 2.22
  
## Notes:
	Max Zoom is locked at 1,000,000x.
	Current Max Iteration is 1000. Any higher leads to some crashes.

## Controls:
	Left Click: Zoom in on cursor point
	Right Click: Zoom out on cursor point
  
## Pictures:

### Mandelbrot Set (Not Zoomed In) 
![Mandelbrot Set (Not Zoomed In)](https://github.com/pleteaud/Mandelbrot-Set-Renderer/blob/master/Pictures/Mandelbrot%20Set.png)

### Mandelbrot Set (Zoomed In) 
![Mandelbrot Set (Zoomed In)](https://raw.githubusercontent.com/pleteaud/Mandelbrot-Set-Renderer/master/Pictures/Zoomed%20In%20Mandelbrot%20Set.png)

## Future Goal: 
	The main idea for this project is for user to select between multiple type of fractals to render. As fractal rendering should then be saved as a picture.
	The user could select to alter his/her background screen to cycle through the pictures.

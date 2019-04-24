#version 400 core
#extension GL_ARB_gpu_shader_fp64: enable
//Redeclare origin pixel coordinate for top leff
//layout(origin_upper_left) in vec4 gl_FragCoord;

out vec4 FragColor;
uniform vec2 windowSize;
uniform vec2 aspectRatio;
uniform sampler2D colorPalette;
uniform vec2 translation; 
uniform float scale;
	
vec4 calcColor(double val) {
	float t = float(val);
    	float r = 10.0 * (1.0 - t) * t * t * t;
    	float g = 25.0 * (1.0 - t) * (1.0 - t) * t * t * t;
    	float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;
    return vec4(r, g, b, 1.0);
}
void main()
{
	vec2 normalizedFragPos = (gl_FragCoord.xy/windowSize); // get the the coordinate within 0-1 i.e normalize the fragment position
	dvec2 scaledFragPos = normalizedFragPos*aspectRatio;
	scaledFragPos -= aspectRatio/2; //Render the fractal at center of coordinate space 	                                                                         .
        scaledFragPos /= scale; //Factor to zoom in or out coordinates.
	scaledFragPos -= translation; //Translate coordinate

	double x = 0.; 
	double y = 0.; 
	double iteration = 0.;
	double maxIteration = 1000.;

	//Mandelbrot set escape algo
	while((x*x + y*y <= (1<<16)) && (iteration < maxIteration))
	{
		double xtemp = x*x - y*y + scaledFragPos .x;
		y = 2*x*y + scaledFragPos .y;
		x = xtemp;
		iteration++;
	}
	//Interpolation 
 	float log_zn = 0;
	float nu;
	float log_2 = log2(2.0);
  	if ( iteration < maxIteration )
	{
    		log_zn = log2( float(x*x + y*y) ) / 2;
    		nu = log2( log_zn / log2(2.) ) / log2(2.);
    		iteration = iteration + 1 - nu;
	}
	//Set Pixel color
	double normalizedIterations = iteration/maxIteration;
	double normalizedIterationsV2 = (iteration+1)/maxIteration;
	vec4 color1 = calcColor(normalizedIterations);
	vec4 color2 = calcColor(normalizedIterationsV2);	
	FragColor = vec4(mix(color2,color1,normalizedIterations));
}
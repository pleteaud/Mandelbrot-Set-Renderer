#include <windows.h>
#include <iostream>
#include <string.h>
#include <algorithm>
#include "Window.h"
#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"
#include "Core.h"
#include "Framebuffer.h"

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR arguments, int show)
{
	float window_size[2];
	glm::u16vec2 prevSize{ 0,0 };
	float prevScale = 0;
	try
	{
		//Create Window
		Window::create("MandelBrot Set", {800, 800}, true);

		Shader shaderHandle("Fractal");

		//Create Rectanlge vertices and Indices for EBO
		std::vector<float> vertices =
		{
			// positions	  
			1.0f, 1.0f, 0.0f, // top right
			1.0f, -1.0f, 0.0f,  // bottom right
			-1.0f, -1.0f, 0.0f, // bottom left
			-1.0f, 1.0f, 0.0f,  // top left
		};
		std::vector<uint32_t> indices{0, 1, 2, 2, 3, 0};

		//Create Shader and Texture
		Vertex square(vertices, indices);
		Texture colorPalette("Resources/Textures/colormap.jpg");
		//Bind Texture
		colorPalette.bindTexture(GL_TEXTURE_2D);

		while(1)
		{	
			glm::u16vec2 *size = Window::getWindowSize();
			float *scale = Window::getScale();
			if (((prevSize.x != size->x) || (prevSize.y != size->y)) || (prevScale != *scale))
			{
				//Copy Window Size to send to fragment shader
				window_size[0] = size->x;
				window_size[1] = size->y;
				//Render Image
				shaderHandle.use();
				shaderHandle.setVec2Float("windowSize", window_size);
				shaderHandle.setVec2Float("aspectRatio", Window::getAspectRatio());
				shaderHandle.setFloat("scale", *scale);
				shaderHandle.setVec2Float("translation", Window::getTranslationArr());
				square.useVertexArrObj(ON);
				shaderHandle.draw(ELEMENT, GL_TRIANGLES, 6);
				prevSize = *size;
				prevScale = *scale;
			}
			if (!Window::update())
				break;;
		}
	}
	catch(Core::Exception& e){ Core::error(e.message); }
	catch(...){ Core::error("Unhandled exception."); }
}


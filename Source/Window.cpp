#include <Windows.h>
#include <WindowsX.h>
#include <GL3W/gl3w.h>
#include <GL3W/wglext.h>
#include <tchar.h>
#include "Window.h"
#include "Core.h"

namespace
{
	constexpr int32_t context_attributes[]{WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3, WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB, WGL_CONTEXT_FLAGS_ARB,
		WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, NULL};

	constexpr int32_t pixel_format_attributes[]{WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE, WGL_DOUBLE_BUFFER_ARB, GL_TRUE, WGL_ACCELERATION_ARB,
		WGL_FULL_ACCELERATION_ARB, WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24, WGL_STENCIL_BITS_ARB, 8, WGL_SAMPLE_BUFFERS_ARB, 1, WGL_SAMPLES_ARB,
		16, NULL};

	const HINSTANCE module_handle{GetModuleHandle(NULL)};

	PIXELFORMATDESCRIPTOR pixel_format_descriptor{sizeof(PIXELFORMATDESCRIPTOR),
		1, PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0};

	FILE *console_output_stream;
	WNDCLASSEX window_information;
	int32_t pixel_format;
	HDC device_context_handle;
	HGLRC opengl_context_handle;
	HWND handle;

	bool open{ true };
	std::string title;
	glm::i16vec2 position;
	glm::u16vec2 size;
	glm::fmat4 orthographic_matrix;
	float scale = .25f;
	bool change{ true };
	float translation[2] = { 0.5f, 0 };
	float aspectRatio[2];
	static LRESULT window_callback(HWND handle, UINT message, WPARAM information, LPARAM informaton_long)
   {
      switch(message)
      {
      //Scale
	  case WM_LBUTTONDOWN:
	  {

		float mouseCoord[2]{ GET_X_LPARAM(informaton_long) + 0.5, GET_Y_LPARAM(informaton_long) + 0.5}; // Get pixel that cursor resides on
		//Flip Y coordinate to OpenGL Coordinate system (Y coordinates  increases upwards)
		mouseCoord[1] = size.y - mouseCoord[1];
			  
		//Determine MandelBrot coordinate under cursor using the old scale and transformation sequence
		float mandelBrotCoord[2]{
		aspectRatio[0] * (mouseCoord[0] / size[0] - 0.5f) / scale - translation[0],
		aspectRatio[1] * (mouseCoord[1] / size[1] - 0.5f) / scale - translation[1]
		};
		//Increase Scale exponentially
		scale *= 1.1;
		//Determine new translation using the mouseCoordinate (pixel location) and the mandelbrot coordinate associated with that pixel
		translation[0] = aspectRatio[0] * (mouseCoord[0] / size[0] - 0.5f) / scale - mandelBrotCoord[0];
		translation[1] = aspectRatio[1] * (mouseCoord[1] / size[1] - 0.5f) / scale - mandelBrotCoord[1]; 
		change = true;
		break;
	  }
	  case WM_RBUTTONDOWN:
	  {
		  
		  float mouseCoord[2]{ GET_X_LPARAM(informaton_long) + 0.5, GET_Y_LPARAM(informaton_long) + 0.5 }; // Get pixel that cursor resides on
		 //Flip Y coordinate to OpenGL Coordinate system (Y coordinates  increases upwards)
		  mouseCoord[1] = size.y - mouseCoord[1];

		  //Determine MandelBrot coordinate under cursor using the old scale and transformation sequence
		  float mandelBrotCoord[2]{
		  aspectRatio[0] * (mouseCoord[0] / size[0] - 0.5f) / scale - translation[0],
		  aspectRatio[1] * (mouseCoord[1] / size[1] - 0.5f) / scale - translation[1]
		  };
		  //Increase Scale exponentially
		  scale /= 1.1;
		  //Determine new translation using the mouseCoordinate (pixel location) and the mandelbrot coordinate associated with that pixel
		  translation[0] = aspectRatio[0] * (mouseCoord[0] / size[0] - 0.5f) / scale - mandelBrotCoord[0];
		  translation[1] = aspectRatio[1] * (mouseCoord[1] / size[1] - 0.5f) / scale - mandelBrotCoord[1];
		  change = true;
		  break;
	  }
      case WM_SIZE:
      {
		glm::u16vec2 size{LOWORD(informaton_long), HIWORD(informaton_long)};
		//Copy size vector
		::size = size;
		change = true;
		//Set the orthographic matrix
        orthographic_matrix = glm::ortho(0.f, static_cast<float>(size.x),
        static_cast<float>(size.y), 0.f, -1.f, 1.f);
		//Set Aspect Ratio
		Window::setAspectRatio();
		//Set the viewport
        glViewport(0, 0, size.x, size.y);
		break;
      }
	  //Close
	  case WM_CLOSE: open = false; break;

		//Default
      default: return DefWindowProc(handle, message, information, informaton_long);
      }

		return NULL;
   }

	void create_window_and_device_context()
	{
		handle = CreateWindow(title.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, position.x,
			position.y, size.x, size.y, NULL, NULL, module_handle, NULL);
		if(!handle) throw Core::Exception{"Could not create the window."};
		device_context_handle = GetDC(handle);
	}

	void destroy_window()
	{
		wglMakeCurrent(NULL, NULL);
		if(opengl_context_handle) wglDeleteContext(opengl_context_handle);
		if(handle && device_context_handle) ReleaseDC(handle, device_context_handle);
		if(handle) DestroyWindow(handle);
	}

	void create_window()
	{
		//Create the dummy window and rendering context
		window_information = {sizeof(WNDCLASSEX), CS_HREDRAW|CS_VREDRAW, window_callback,
			0, 0, module_handle, NULL, NULL, NULL, NULL, title.c_str(), NULL};
		RegisterClassEx(&window_information);

		create_window_and_device_context();
		pixel_format = ChoosePixelFormat(device_context_handle, &pixel_format_descriptor);

		if(!pixel_format) throw Core::Exception{"Could not get a valid base pixel format."};
		if(!SetPixelFormat(device_context_handle, pixel_format, &pixel_format_descriptor))
			throw Core::Exception{"Could not set the base pixel format."};

		opengl_context_handle = wglCreateContext(device_context_handle);
		if(!device_context_handle) throw Core::Exception{
			"Could not create the base rendering context."};
		if(!wglMakeCurrent(device_context_handle, opengl_context_handle))
			throw Core::Exception{"Could not activate the base rendering context."};

		//Get the OpenGL function pointers
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB{reinterpret_cast<
			PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"))};
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB{reinterpret_cast<
			PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"))};

		//Destroy the dummy window and context
		destroy_window();

		//Create the window and the OpenGL 3.3 context
		create_window_and_device_context();

		uint32_t format_count;
		if(!wglChoosePixelFormatARB(device_context_handle,
			pixel_format_attributes, nullptr, 1, &pixel_format, &format_count))
			throw Core::Exception{"Could not get a valid OpenGL 3.3 pixel format."};
		DescribePixelFormat(device_context_handle, pixel_format,
			sizeof(pixel_format_descriptor), &pixel_format_descriptor);
		if(!SetPixelFormat(device_context_handle, pixel_format, &pixel_format_descriptor))
			throw Core::Exception{"Could not set the OpenGL 3.3 pixel format."};

		opengl_context_handle = wglCreateContextAttribsARB(
			device_context_handle, NULL, context_attributes);
		if(!opengl_context_handle) throw Core::Exception{
			"Could not create the OpenGL 3.3 rendering context."};
		if(!wglMakeCurrent(device_context_handle, opengl_context_handle))
			throw Core::Exception{"Could not activate the OpenGL 3.3 rendering context."};

		//Initialize GL3W
		if(gl3wInit()) throw Core::Exception{"Could not initialize OpenGL."};
		// Initialize MSAA
		glEnable(GL_MULTISAMPLE);
	}
}

void Window::create(const std::string& title, const glm::u16vec2& size, bool console)
{
	::title = title;
	::size = size;

	//Create the console
	if(console)
	{
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		freopen_s(&console_output_stream, "CONOUT$", "w", stdout);
	}

	//Create the window
	create_window();
	ShowWindow(handle, SW_SHOW);
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

bool Window::update()
{
	if (change) 
	{
		//Swap the buffers
		SwapBuffers(device_context_handle);

		//Clear the buffer if size changed
		glClearColor(.2f, .3f, .3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		change = false;
	}
	//Process the messages
	static MSG msg;
	while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) DispatchMessage(&msg);
	return open;
}

void Window::setAspectRatio()
{
	float maxUnit = static_cast<float>(std::max(size.x, size.y));
	aspectRatio[0] = size.x / maxUnit;
	aspectRatio[1] = size.y / maxUnit;
}
float * Window::getAspectRatio()
{
	return aspectRatio;

}
glm::u16vec2* Window::getWindowSize()
{
	return &size;
}

float * Window::getScale()
{
	return &scale;
}

float * Window::getTranslationArr()
{
	return translation;
}

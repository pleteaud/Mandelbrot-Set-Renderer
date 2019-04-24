#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include <GL3W/gl3w.h>
#include <GL3W/wglext.h>
#include <iostream>

#define EMPTY_TEXTURE_BUFFER (0x00)
#define MSAA_TEXTURE_BUFFER  (0x01)
class Texture
{
private:
	unsigned int ID;

public:
	Texture() {}
	Texture(int width, int height, uint8_t bufferType);
	Texture(std::string imageLocation);
	//unsigned int getID() { return ID; }
	void bindTexture(int textureType);
	void unbindTexture(int textureType);
	unsigned int getID() { return ID; }
};
#endif
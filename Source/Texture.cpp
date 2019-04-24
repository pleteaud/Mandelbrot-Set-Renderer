
#include "Texture.h"
#include "Core.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB Image/stb_image.h>

void Texture::bindTexture(int textureType){ glBindTexture(textureType, ID); }
void Texture::unbindTexture(int textureType) { glBindTexture(textureType, 0); }


Texture::Texture(std::string imageLocation)
{
	glGenTextures(1, &ID);
	bindTexture(GL_TEXTURE_2D);

	//Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load and generate the texture
	int width, height, channels;
	uint8_t *data{stbi_load(imageLocation.c_str(), &width, &height, &channels, STBI_rgb_alpha)};
	if(!data) throw Core::Exception{"Failed to load texture \""+imageLocation+"\""};

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//Generate all mipmaps for texture; Ask Myles to explain if you get a chance
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}
Texture::Texture(int width, int height, uint8_t bufferType)
{
	int samples = 4;
	if (bufferType)
	{
		glGenTextures(1, &ID);
		bindTexture(GL_TEXTURE_2D_MULTISAMPLE);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
		unbindTexture(GL_TEXTURE_2D_MULTISAMPLE);
	}
	else
	{
		glGenTextures(1, &ID);
		bindTexture(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unbindTexture(GL_TEXTURE_2D);
	}
}
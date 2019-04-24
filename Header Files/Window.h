#pragma once
#include <string>
#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"
namespace Window
{
	void create(const std::string& title, const glm::u16vec2& size, bool console = false);
	bool update();
	glm::u16vec2* getWindowSize();
	float *getScale();
	float *getTranslationArr();
	float *getAspectRatio();
	void setAspectRatio();
}
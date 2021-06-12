#pragma once
#include <glad/glad.h>

#ifndef TEXTURE_H
#define TEXTURE_H
namespace TowerDelivery {

	GLuint loadDDS(const char* imagepath);

	unsigned int loadTexture(char const* path);
}
#endif

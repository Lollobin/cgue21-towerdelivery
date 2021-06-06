#pragma once
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "TowerDelivery/Rendering/Shader.h"
#include <TowerDelivery.h>

namespace TowerDeliver {

	class TextRendering {
	public:
		TextRendering(TowerDelivery::Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
	};
}
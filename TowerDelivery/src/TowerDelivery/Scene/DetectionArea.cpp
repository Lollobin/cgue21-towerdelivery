#include "tdpch.h"
#include "DetectionArea.h"

TowerDelivery::DetectionArea::DetectionArea(glm::vec3 position, float width, float height, float depth)
	: m_position(position), m_width(width / 2.0f), m_height(height / 2.0f), m_depth(depth / 2.0f)
{
}

TowerDelivery::DetectionArea::~DetectionArea()
{
}

bool TowerDelivery::DetectionArea::Contains(glm::vec3 position)
{
	float x = position[0];
	float y = position[1];
	float z = position[2];

	if (x > m_position[0] - m_width && x < m_position[0] + m_width
		&& y > m_position[1] - m_height && y < m_position[1] + m_height
		&& z > m_position[2] - m_depth && z < m_position[2] + m_depth) {
		return true;
	}

	return false;
}

bool TowerDelivery::DetectionArea::Contains(CharacterController* character)
{
	return Contains(character->GetPosition());
}
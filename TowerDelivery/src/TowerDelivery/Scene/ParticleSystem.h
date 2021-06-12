#pragma once

#include <algorithm>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "TowerDelivery/Core/Timestep.h"

namespace TowerDelivery {
	// CPU representation of a particle
	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a; // Color
		float size, angle, weight;
		float life; // Remaining life of the particle. if < 0 : dead and unused.
		float cameradistance;

		bool operator<(Particle& that) {
			// Sort in reverse order : far particles drawn first.
			return this->cameradistance > that.cameradistance;
		}
	};

	class ParticleSystem
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		void OnUpdate(Timestep ts, glm::vec3 CameraPosition);
		void Draw();

	private:
		unsigned int ParticlesCount = 0;
		const int MaxParticles = 10000;

		Particle ParticlesContainer[10000];

		inline static GLfloat* g_particule_position_size_data = new GLfloat[10000 * 4];
		inline static GLubyte* g_particule_color_data = new GLubyte[10000 * 4];

		GLuint billboard_vertex_buffer;
		GLuint particles_position_buffer;
		GLuint particles_color_buffer;

		int LastUsedParticle = 0;

	private:
		int FindUnusedParticle();
		void SortParticles();
	};
}

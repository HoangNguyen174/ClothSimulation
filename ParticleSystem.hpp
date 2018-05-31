#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include "GameCommon.hpp"
#include <vector>
#include "EmitterStrategy.hpp"
#include "ExplosionEmitterStrategy.hpp"
#include "IntegrationMethodStrategy.hpp"
#include "ForwardEulerIntegrationStrategy.hpp"
#include "ForceModelStrategy.hpp"
#include "Particle.hpp"
#include "Texture.hpp"

const int MAX_NUMBER_PARTICLE = 1000;

class ParticleSystem
{
	public:
		std::vector<Particle> m_particleList;
		float m_particleSize;
		EmitterStrategy* m_emitterStrategy;
		IntegrationMethodStrategy* m_integrationMethod;
		bool m_discreteUpdate;
		bool m_isSystemLiveForever;
		Texture* m_texture;

	public:
		ParticleSystem() { m_particleSize = 0.01f;
						   m_discreteUpdate = true;
						   m_isSystemLiveForever = false; }
		void Initialize();
		void Update(float elapsedTime);
		void Render();
		void RenderParticle();
};

#endif
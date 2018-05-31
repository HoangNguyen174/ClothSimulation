#include "Particle.hpp"

Particle::Particle()
{
	m_mass = 1.f;
	m_lifeSpanInSecond = 0.f;
	m_color = RGBColor(1.f,0.f,0.f,1.f);
	m_isLiveForever = false;
}

#ifndef PARTICLE_H
#define PARTICLE_H
#include "GameCommon.hpp"

class Particle
{
	public:
		Vector3 m_position;
		Vector3 m_velocity;
		Vector3 m_acceleration;
		float m_mass;
		float m_lifeSpanInSecond;
		RGBColor m_color;
		float m_size;
		bool m_isLiveForever;

	public:
		Particle();
		~Particle() {};
};


#endif
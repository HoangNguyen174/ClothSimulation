#ifndef SMOKE_EMITTER_H
#define SMOKE_EMITTER_H
#include "Vector3.h"
#include "EmitterStrategy.hpp"
#include "Particle.hpp"

class SmokeEmitterStrategy : public EmitterStrategy
{
	public:
		Vector3 m_initPosition;

	public:
		SmokeEmitterStrategy(const Vector3& pos)
		{
			m_initPosition = pos;
		}

		void Initialize( Particle& particle )
		{
			//boost::mt19937 randomGenerator( time ( 0 ));
			boost::random::uniform_real_distribution<> randomRadius( 0, 1);
			boost::random::uniform_real_distribution<> randomLifeSpan( 2.0, 4.0);
			boost::random::uniform_real_distribution<> randomAngle( 0, 360 );
			boost::random::uniform_real_distribution<> randomHeight( 0, 2 );
			boost::random::uniform_real_distribution<> randomSize( 1, 5 );

			boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomAngle( randomGenerator, randomAngle );
			boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomRadius( randomGenerator, randomRadius );
			boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomLifeSpan( randomGenerator, randomLifeSpan );
			boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomHeight(randomGenerator, randomHeight);
			boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomSize(randomGenerator, randomSize);

			particle.m_lifeSpanInSecond = (float)generateRandomLifeSpan();
			particle.m_position = m_initPosition;
			float angle = (float)generateRandomAngle();
			float radius = (float)generateRandomRadius();
			float size = (float)generateRandomSize();
			particle.m_size = size;
			particle.m_velocity.z = (float)generateRandomHeight();
			particle.m_velocity.x = radius * cos(MathUtilities::DegToRad(angle));
			particle.m_velocity.y = radius * sin(MathUtilities::DegToRad(angle));

		}
};

#endif
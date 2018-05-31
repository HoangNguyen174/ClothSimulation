#ifndef EXPLOSION_EMITTER_STRATEGY_H
#define EXPLOSION_EMITTER_STRATEGY_H
#include "Vector3.h"
#include "EmitterStrategy.hpp"
#include "Particle.hpp"

class ExplosionEmitterStrategy : public EmitterStrategy
{
	public:
		Vector3 m_initPosition;

	public:
		ExplosionEmitterStrategy(const Vector3& pos)
		{
			m_initPosition = pos;
		}

		void Initialize( Particle& particle )
		{
			boost::random::uniform_real_distribution<> randomPolar( -90, 90);
			boost::random::uniform_real_distribution<> randomAzimuthal( 1.0, 360.0);
			boost::random::uniform_real_distribution<> randomRadius( 0.5, 1.0);
			boost::random::uniform_real_distribution<> randomLifeSpan( 2.0, 4.0);
			boost::random::uniform_real_distribution<> randomSize( 1, 5 );

			boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomPolar( randomGenerator, randomPolar );
			boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomAzimuthal( randomGenerator, randomAzimuthal );
			boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomRadius( randomGenerator, randomRadius );
			boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomLifeSpan( randomGenerator, randomLifeSpan );
			boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomSize(randomGenerator, randomSize);

			particle.m_lifeSpanInSecond = (float)generateRandomLifeSpan();
			particle.m_position = m_initPosition;
			float polar = (float)generateRandomPolar();
			float azimuthal = (float)generateRandomAzimuthal();
			float radius = (float)generateRandomRadius();
			float size = (float)generateRandomSize();
			particle.m_size = size;
			particle.m_velocity = Vector3::ConvertFromSphericalCoordToCartersianCoord(radius,azimuthal,polar) ;

		};
};

#endif
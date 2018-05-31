#ifndef EMITTER_STRATEGY_H
#define EMITTER_STRATEGY_H
#include "Particle.hpp"
#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>

class EmitterStrategy
{
	public:
		boost::mt19937 randomGenerator;

	public:
		EmitterStrategy() {};
		~EmitterStrategy() {};
		virtual void Initialize(Particle& particle) {};
};

#endif
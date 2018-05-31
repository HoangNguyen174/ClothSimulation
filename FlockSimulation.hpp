#ifndef FLOCK_SIMULATION
#define FLOCK_SIMULATION
#include "ParticleSystem.hpp"
#include "Vector3.h"
#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>

const int FLOCK_SIZE = 200;
const int PREDATOR_SIZE = 10;
const int MAX_TRAVEL_RADIUS = 1;
const float MAX_SPEED = 2.f;
const float MIN_SPEED = 1.5f;
const float PREDATOR_RADIUS = 1.5f;
class FlockSimulation
{
	public:
		std::vector<Particle> m_flockList;
		std::vector<Particle> m_predatorList;
		Vector3 m_centerPoint;
		Vector3 m_centerOfMass;
		boost::mt19937 randomGenerator;

	public:
		FlockSimulation();
		void PullParticleToCenterPoint(Particle& particle);
		void ComputeAndApplyForceModelToFlock(Particle& particle, int index);
		void ComputeAndApplyForceModelToPredator(Particle& particle, int index);
		void Render();
		void Update(float elapsedTime);
		void ConstrainVelocity(Particle& particle);
		void Initialize(Particle& particle);
		Vector3 ComputeCenterOfMass();
};

#endif
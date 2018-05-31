#ifndef GRAVITATIONAL_FORCE
#define GRAVITATIONAL_FORCE
#include "ForceModelStrategy.hpp"

const float GRAVITATIONAL_ACCEL = 2.8f;

class GravitationalForce : public ForceModelStrategy
{
	public:
		void ComputeForceModel(float timeStep, const float* state, float* stateFirstDev)
		{
			stateFirstDev[0] = state[3];
			stateFirstDev[1] = state[4];
			stateFirstDev[2] = state[5];

			stateFirstDev[3] =  0;
			stateFirstDev[4] =  0;
			stateFirstDev[5] =  -GRAVITATIONAL_ACCEL;
		}
};

#endif
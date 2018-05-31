#ifndef FORCE_MODEL_STRATEGY
#define FORCE_MODEL_STRATEGY
#include "GameCommon.hpp"

class ForceModelStrategy
{
	public:
		virtual void ComputeForceModel(float timeStep, const float* state, float* stateFirstDev) 
		{
			stateFirstDev[0] = state[3];
			stateFirstDev[1] = state[4];
			stateFirstDev[2] = state[5];

			stateFirstDev[3] = 0.f;
			stateFirstDev[4] = 0.f;
			stateFirstDev[5] = 0.f;
		};
};

#endif
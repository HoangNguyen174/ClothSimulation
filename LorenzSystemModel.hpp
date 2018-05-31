#ifndef LORENZ_SYSTEM 
#define LORENZ_SYSTEM
#include "ForceModelStrategy.hpp"

class LorenzSystem : public ForceModelStrategy
{
	public:
		float m_alpha;
		float m_penta;
		float m_beta;

	public:
		LorenzSystem()
		{
		   m_alpha = 10.f;
		   m_penta = 28.f;
		   m_beta = 8.f / 3.f; 
		}

		void ComputeForceModel(float timeStep, const float* state, float* stateFirstDev)
		{
			stateFirstDev[0] = state[3];
			stateFirstDev[1] = state[4];
			stateFirstDev[2] = state[5];

			stateFirstDev[3] = 0.1f * ( m_alpha * ( state[1] - state[0] ) );
			stateFirstDev[4] = 0.1f * ( state[0] * ( m_penta - state[2] ) - state[1] );
			stateFirstDev[5] = 0.1f * ( state[0] * state[1] - m_beta * state[2] );
		}
};

#endif
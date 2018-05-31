#ifndef WINDFORCE_MODEL_H
#define WINDFORCE_MODEL_H
#include "ForceModelStrategy.hpp"

class WindForce : public ForceModelStrategy
{
	public:
		Vector3 m_windForce;
		float m_damping;
		float m_g;

	public:
		WindForce()
		{
			m_windForce = Vector3 ( 0.f,-10.f, 1.f);
			m_damping = 1.f;
			m_g = -9.8f;
		}

		void ComputeForceModel(float timeStep, const float* state, float* stateFirstDev)
		{
			stateFirstDev[0] = state[3];
			stateFirstDev[1] = state[4];
			stateFirstDev[2] = state[5];

			stateFirstDev[3] =  -m_damping * ( state[3] - m_windForce.x ) ;
			stateFirstDev[4] =  -m_damping * ( state[4] - m_windForce.y ) ;
			stateFirstDev[5] =  m_g - m_damping * ( state[5] - m_windForce.z ) ;
		}
};

#endif

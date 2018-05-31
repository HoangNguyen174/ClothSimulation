#ifndef INTEGRATION_METHOD
#define INTEGRATION_METHOD
#include "GameCommon.hpp"
#include "ForceModelStrategy.hpp"

class IntegrationMethodStrategy
{
	public:
		ForceModelStrategy* m_forceModel;

	public:
		virtual void Integrate(Vector3* initPosition, Vector3* initVelocity, float timeStepInSecond) 
		{

		};
};

#endif
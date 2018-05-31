#ifndef FORWARD_EULER
#define FORWARD_EULER
#include "IntegrationMethodStrategy.hpp"
#include "ForceModelStrategy.hpp"

class ForwardEulerIntegrationMethod : public IntegrationMethodStrategy
{
	public:
		void Integrate(Vector3* initPosition, Vector3* initVelocity,float timeStep)
		{
			float S[6];
			float DS[6];
			float NextS[6];
			timeStep = 0.01f;
			S[0] = initPosition->x;
			S[1] = initPosition->y;
			S[2] = initPosition->z;

			S[3] = initVelocity->x;
			S[4] = initVelocity->y;
			S[5] = initVelocity->z;

			m_forceModel->ComputeForceModel(timeStep,S, DS);

			NextS[0] = S[0] + DS[0] * timeStep;
			NextS[1] = S[1] + DS[1] * timeStep;
			NextS[2] = S[2] + DS[2] * timeStep;

			NextS[3] = S[3] + DS[3] * timeStep;
			NextS[4] = S[4] + DS[4] * timeStep;
			NextS[5] = S[5] + DS[5] * timeStep;

			S[0] = NextS[0];
			S[1] = NextS[1];
			S[2] = NextS[2];
			S[3] = NextS[3];
			S[4] = NextS[4];
			S[5] = NextS[5];

			initPosition->x = S[0];
			initPosition->y = S[1];
			initPosition->z = S[2];

			initVelocity->x = S[3];
			initVelocity->y = S[4];
			initVelocity->z = S[5];
		}
};

#endif
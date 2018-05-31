#include "ClothSimulation.hpp"

ClothSimulation::ClothSimulation()
{
	Intialize();
	DebugSphere s1;
	DebugSphere s2;
	DebugSphere s3;
	DebugSphere s4;
	s1.m_center = Vector3( 1.f,1.f,1.5f );
	s1.m_radius = 0.5f;
	s1.m_velocity = Vector3(0.f,0.f,0.f);
	s1.m_color = RGBColor(1.f,0.f,0.f,1.f);

	s2.m_center = Vector3( 2.0f,1.f,1.f );
	s2.m_radius = 0.5f;
	s2.m_color = RGBColor(1.f,1.f,0.f,1.f);

	s3.m_center = Vector3( 1.0f,2.0f,0.f );
	s3.m_radius = 0.5f;
	s3.m_color = RGBColor(1.f,0.f,1.f,1.5f);

	s4.m_center = Vector3( 0.75f,0.75f,0.f );
	s4.m_radius = 0.5f;
	s4.m_color = RGBColor(0.f,1.f,1.f,1.f);

	s1.m_drawMode = DepthTestOn;
	s2.m_drawMode = DepthTestOn;
	s3.m_drawMode = DepthTestOn;
	s4.m_drawMode = DepthTestOn;
	s1.m_velocity = s2.m_velocity = s3.m_velocity = s4.m_velocity;
	m_sphereList.push_back(s1);
	m_sphereList.push_back(s2);
	m_sphereList.push_back(s3);
	m_sphereList.push_back(s4);
	m_texture = Texture::CreateOrGetTexture("HordeFlag.jpg");
	m_pickingIndex1 = -1;
	m_pickingIndex2 = -1;
	m_fixP1 = 0;
	m_fixP2 = CLOTH_WIDTH_X - 1;
	m_fixP3 = CLOTH_SIZE - CLOTH_WIDTH_X;
	m_fixP4 = CLOTH_SIZE - 1 ;
	m_windForce = Vector3(0.f,0.f,0.f);
}

void ClothSimulation::Intialize()
{
	m_texWidth = 1.f / CLOTH_WIDTH_X;
	m_texHeight = 1.f / CLOTH_HEIGHT_Y;

	Particle temp;
	for(unsigned int y = 0; y < CLOTH_HEIGHT_Y; y++ )
	{
		for(unsigned int x = 0; x < CLOTH_WIDTH_X; x++)
		{	
			temp.m_position.x = DIST_BETWEEN_POINT * x;
			temp.m_position.y = DIST_BETWEEN_POINT * y;
			temp.m_position.z = INITIAL_HEIGHT;
			m_clothParticleList.push_back(temp);
		}
	}

	//Add Horizontal Structure Spring
	for(unsigned int y = 0; y < CLOTH_HEIGHT_Y; y++)
	{
		for(unsigned int x = 0; x < CLOTH_WIDTH_X - 1; x++)
		{	
			AddSpring((y * CLOTH_WIDTH_X) + x, (y * CLOTH_WIDTH_X) + x + 1,KS_STRUCT, KD_STRUCT,STRUCTURAL_SPRING);
		}
	}
	
	//Add Vertical Structure Spring
	for(unsigned int y = 0; y < CLOTH_HEIGHT_Y - 1; y++)
	{
		for(unsigned int x = 0; x < CLOTH_WIDTH_X; x++)
		{	
			AddSpring( x + (y * CLOTH_WIDTH_X) , x + ( (y + 1) * CLOTH_WIDTH_X ), KS_STRUCT,KD_STRUCT,STRUCTURAL_SPRING);
		}
	}

	//Add Shear Spring
	for(unsigned int y = 0; y < CLOTH_HEIGHT_Y - 1; y++)
	{
		for(unsigned int x = 0; x < CLOTH_WIDTH_X - 1; x++)
		{	
			AddSpring( x + (y * CLOTH_WIDTH_X) , x + 1 + ( (y + 1) * CLOTH_WIDTH_X ), KS_SHEAR, KD_SHEAR,SHEAR_SPRING);
			AddSpring( x + ( (y + 1) * CLOTH_WIDTH_X ), ( x + 1 ) + (y * CLOTH_WIDTH_X), KS_SHEAR, KD_SHEAR,SHEAR_SPRING);
		}
	}
	
	//Add Horizontal Bend Spring
	for(unsigned int y = 0; y < CLOTH_HEIGHT_Y; y++)
	{
		for(unsigned int x = 0; x < CLOTH_WIDTH_X - 2; x++)
		{	
			AddSpring( x + (y * CLOTH_WIDTH_X) , x + 2 + ( y * CLOTH_WIDTH_X ), KS_BEND,KD_BEND,BEND_SPRING);
		}
	}

	//Add Vertical Bend Spring
	for(unsigned int y = 0; y < CLOTH_HEIGHT_Y - 2; y++)
	{
		for(unsigned int x = 0; x < CLOTH_WIDTH_X ; x++)
		{	
			AddSpring( x + (y * CLOTH_WIDTH_X) , x + ( (y + 2) * CLOTH_WIDTH_X ), KS_BEND,KD_BEND,BEND_SPRING);
		}
	}
	
	for(unsigned int i = 0; i < m_clothParticleList.size(); i++)
	{
		Vector3 temp;
		m_forceOnClothPartcleList.push_back(temp);
	}

}

void ClothSimulation::Render()
{
	float offset = 0.f;
	glEnable(GL_POINT_SMOOTH);
	glPointSize(3.f);
	
// 	glBegin(GL_POINTS);
// 	glColor4f(1.f,0.f,0.f,1.f);
// 	glBindTexture( GL_TEXTURE_2D, m_texture->m_openglTextureID);
// 	for(unsigned int i = 0; i < m_clothParticleList.size(); i++)
// 	{
// 		Particle& point = m_clothParticleList[i];
// 		Vector3& pos = point.m_position;
// 		glVertex3f( pos.x, pos.y, pos.z );
// 	}
// 	glEnd();
	
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor4f(1.f,1.f,1.f,1.f);
	glBindTexture( GL_TEXTURE_2D, m_texture->m_openglTextureID);
	for(unsigned int y = 0; y < CLOTH_HEIGHT_Y - 1; y++)
	{
		for(unsigned int x = 0; x < CLOTH_WIDTH_X - 1; x++)
		{
			int index = x + y * CLOTH_WIDTH_X;

			glTexCoord2f( x * m_texWidth, y * m_texHeight );
			glVertex3f( m_clothParticleList[index].m_position.x, m_clothParticleList[index].m_position.y,m_clothParticleList[index].m_position.z);

			glTexCoord2f( ( x + 1) * m_texWidth, y * m_texHeight );
			glVertex3f( m_clothParticleList[index + 1].m_position.x, m_clothParticleList[index + 1].m_position.y,m_clothParticleList[index + 1].m_position.z);

			glTexCoord2f( ( x + 1 ) * m_texWidth, ( y + 1 ) * m_texHeight );
			glVertex3f( m_clothParticleList[index + CLOTH_WIDTH_X + 1].m_position.x, m_clothParticleList[index + CLOTH_WIDTH_X + 1].m_position.y,m_clothParticleList[index + CLOTH_WIDTH_X + 1].m_position.z);

			glTexCoord2f( x * m_texWidth, ( y + 1 ) * m_texHeight );
			glVertex3f( m_clothParticleList[index + CLOTH_WIDTH_X ].m_position.x, m_clothParticleList[index + CLOTH_WIDTH_X ].m_position.y,m_clothParticleList[index + CLOTH_WIDTH_X ].m_position.z);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	for(unsigned int i = 0; i < m_sphereList.size(); i++)
	{
		m_sphereList[i].Render();
	}
	
// 	glBegin(GL_LINES);
// 	for(unsigned int i = 0; i < m_springList.size(); i++)
// 	{
// 		Spring& spring = m_springList[i];
// 		int& p1 = m_springList[i].m_endPoint1Index;
// 		int& p2 = m_springList[i].m_endPoint2Index;
// 		Vector3& pos1 = m_clothParticleList[p1].m_position;
// 		Vector3& pos2 = m_clothParticleList[p2].m_position;
// 		if(spring.m_type == STRUCTURAL_SPRING)
// 			glColor4f(0.f,0.f,1.f,1.f);
// 		else if(spring.m_type == SHEAR_SPRING)
// 			glColor4f(0.f,1.f,0.f,1.f);
// 		else
// 		{
// 			glColor4f(1.f,1.f,0.f,1.f);
// 			offset = -0.01f;
// 		}
// 
// 		glVertex3f( pos1.x, pos1.y, pos1.z + offset);
// 		glVertex3f( pos2.x, pos2.y, pos2.z + offset);
// 	}
// 	glEnd();

}

void ClothSimulation::AddSpring(int endP1Index, int endP2Index, float ks, float kd, StringType type)
{
	Spring spring;
	spring.m_endPoint1Index = endP1Index;
	spring.m_endPoint2Index = endP2Index;
	spring.m_ks = ks;
	spring.m_kd = kd;
	spring.m_type = type;
	spring.m_restLength = (m_clothParticleList[endP1Index].m_position - m_clothParticleList[endP2Index].m_position).CalcLength();
	m_springList.push_back(spring);
}

void ClothSimulation::ComputeForce()
{
	for(unsigned int j = 0; j < m_forceOnClothPartcleList.size(); j++)
	{
		if( j != 0 && j != CLOTH_WIDTH_X - 1  && j != CLOTH_SIZE - CLOTH_WIDTH_X && j != CLOTH_SIZE - 1  )
		{	
			m_forceOnClothPartcleList[j] = GRAVITY;
			m_forceOnClothPartcleList[j] += Vector3 ( 0.0f, 0.0f, 2.f);
			m_forceOnClothPartcleList[j] += DAMPING * m_clothParticleList[j].m_velocity;
		}
	}

	for(unsigned int i = 0; i < m_springList.size();i ++)
	{
		Spring& spring = m_springList[i];

		int index1 = spring.m_endPoint1Index;
		int index2 = spring.m_endPoint2Index;

		Vector3 p1 = m_clothParticleList[index1].m_position;
		Vector3 p2 = m_clothParticleList[index2].m_position;

		Vector3 v1 = m_clothParticleList[index1].m_velocity;
		Vector3 v2 = m_clothParticleList[index2].m_velocity;

		Vector3 deltaP = p1 - p2;
		Vector3 deltaV = v1 - v2;

		float deltaPlength = deltaP.CalcLength();
		float dotPV = deltaP.DotProduct(deltaV);
		Vector3 deltaPNormal = deltaP.Normalize();
		
		float leftTerm = -spring.m_ks * ( deltaPlength - spring.m_restLength );
		float rightTerm = spring.m_kd * dotPV / deltaPlength;
		Vector3 force = ( leftTerm + rightTerm ) * deltaPNormal;

		if( index1 != 0 && index1 != CLOTH_WIDTH_X - 1 && index1 != CLOTH_SIZE - CLOTH_WIDTH_X && index1 != CLOTH_SIZE - 1  )
			m_forceOnClothPartcleList[index1] += force;
		if( index2 != 0  && index2 != CLOTH_WIDTH_X - 1 && index2 != CLOTH_SIZE - CLOTH_WIDTH_X && index2 != CLOTH_SIZE - 1 )
			m_forceOnClothPartcleList[index2] -= force;
	}
}

void ClothSimulation::IntergrateEuler(Vector3& pos, Vector3& vel,int index, float timeStep)
{
	pos += ( vel * timeStep );
	vel += ( m_forceOnClothPartcleList[index] * timeStep );
}

void ClothSimulation::IntergrateRK4(float timeStep)
{
	float halfDeltaT  = timeStep / 2.0f;
	float thirdDeltaT = 1 / 3.0f;
	float sixthDeltaT = 1 / 6.0f;
	unsigned int i = 0;
	vector<Vector3> sumF(m_clothParticleList.size(),Vector3(0.f,0.f,0.f));
	vector<Vector3> sumV(m_clothParticleList.size(),Vector3(0.f,0.f,0.f));

	ComputeForce();

	for( i = 0; i < m_clothParticleList.size(); i++) 
	{
		sumF[i] = (m_forceOnClothPartcleList[i] * halfDeltaT) * sixthDeltaT;
		sumV[i] = halfDeltaT * m_clothParticleList[i].m_velocity  * sixthDeltaT;
	}

	ComputeForce();

	for( i = 0; i < m_clothParticleList.size(); i++) 
	{
		sumF[i] = (m_forceOnClothPartcleList[i] * halfDeltaT) * thirdDeltaT;
		sumV[i] = halfDeltaT * m_clothParticleList[i].m_velocity  * thirdDeltaT;
	}

	ComputeForce();

	for( i = 0; i < m_clothParticleList.size(); i++) 
	{
		sumF[i] = (m_forceOnClothPartcleList[i] * timeStep) * thirdDeltaT;
		sumV[i] = timeStep * m_clothParticleList[i].m_velocity  * thirdDeltaT;
	}

	ComputeForce();

	for( i = 0; i < m_clothParticleList.size(); i++) 
	{
		sumF[i] = (m_forceOnClothPartcleList[i] * timeStep) * sixthDeltaT;
		sumV[i] = timeStep * m_clothParticleList[i].m_velocity  * sixthDeltaT;
	}

	for( i = 0; i < m_clothParticleList.size(); i++ )
	{
		m_clothParticleList[i].m_velocity += sumF[i];
		m_clothParticleList[i].m_position += sumV[i];
	}
}

void ClothSimulation::Update(float elapsedTime)
{
	
// 	for( unsigned int i = 0; i < m_clothParticleList.size(); i++ )
// 	{
// 		float k = elapsedTime;
// 		if(m_clothParticleList[i].m_position.z > -1)
// 		{
// 			IntergrateEuler( m_clothParticleList[i].m_position, m_clothParticleList[i].m_velocity,i, 0.05f );
// 		}
// 	}
// 	ComputeExternalForce();
	//ApplyConstrain();
	//ComputeForce();
	
	//IntergrateRK4(.04);//elapsedTime);
	//IntergrateRK4StateSpaceMethod(0.04f);
	//IntergrateEulerStateSpadeMethod(0.05f);
 	for(unsigned int i = 0; i < m_clothParticleList.size(); i++)
 	{
 		if(m_clothParticleList[i].m_position.z <= -0.5f)
 			m_clothParticleList[i].m_velocity.z = 0.f;
 	}
	IntergrateEulerAndResolveCollistion(0.05f);

	DeleteConnectionAtPickingPoint();
}

Vector2 ClothSimulation::ConvertIndexTo2DIndex(int index)
{
	return Vector2 ( index % CLOTH_WIDTH_X, (float)index / CLOTH_WIDTH_X ) ;
}

void ClothSimulation::CompuateForceStateSpaceMethod(const std::vector<StateSpace>& state, std::vector<StateSpace>& stateDev)
{
	static float time = 0.f;
	time += 0.001f;
	for(unsigned int k = 0; k < stateDev.size(); k++)
	{
		if( k != m_fixP1 && k != m_fixP2 &&  k != m_fixP3 && k != m_fixP4  )
		{
			stateDev[k].m_firstComponent = state[k].m_secondComponent;
			stateDev[k].m_secondComponent = GRAVITY ;
			stateDev[k].m_secondComponent +=  m_windForce;
			stateDev[k].m_secondComponent += DAMPING * state[k].m_secondComponent;
		}
	}
	
	for(unsigned int i = 0; i < state.size(); i++)
	{
		Vector3 force;
		
		for(unsigned int j = 0; j < m_springList.size(); j++)
		{
			if( i == m_springList[j].m_endPoint1Index )
			{
				Spring& spring = m_springList[j];

				int index1 = spring.m_endPoint1Index;
				int index2 = spring.m_endPoint2Index;

				Vector3 p1 = state[index1].m_firstComponent;
				Vector3 p2 = state[index2].m_firstComponent;
				
				Vector3 v1 = state[index1].m_secondComponent;
				Vector3 v2 = state[index2].m_secondComponent;

				Vector3 deltaP = p1 - p2;
				Vector3 deltaV = v1 - v2;

				float deltaPlength = deltaP.CalcLength();
				float dotPV = deltaP.DotProduct(deltaV);
				Vector3 deltaPNormal = deltaP.Normalize();

				float leftTerm = -spring.m_ks * ( deltaPlength - spring.m_restLength );
				float rightTerm = spring.m_kd * dotPV / deltaPlength;

				force = ( ( leftTerm + rightTerm ) * deltaPNormal );

				if( i != m_fixP1 && i != m_fixP2  && i != m_fixP3 && i != m_fixP4  )
					stateDev[i].m_secondComponent += force;
				if( index2 != m_fixP1 && index2 != m_fixP2 && index2 != m_fixP3 && index2 != m_fixP4  )
				{
					stateDev[index2].m_firstComponent = state[index2].m_secondComponent;
					stateDev[index2].m_secondComponent -= force;
				}
			 }
		 }
	}
}

void ClothSimulation::IntergrateEulerStateSpadeMethod(float timeStep)
{
	int i = 0;
	hitInfo hit;

	vector<StateSpace> DS(CLOTH_SIZE);
	vector<StateSpace> S0(CLOTH_SIZE);
	vector<Vector3> currPos(CLOTH_SIZE);
	vector<Vector3> nextPos(CLOTH_SIZE);

	for(i = 0; i < CLOTH_SIZE; i++)
	{
		S0[i].m_firstComponent = m_clothParticleList[i].m_position;
		S0[i].m_secondComponent = m_clothParticleList[i].m_velocity;
		currPos[i] = m_clothParticleList[i].m_position;
	}

	CompuateForceStateSpaceMethod(S0,DS);
	
	for(i = 0; i < CLOTH_SIZE; i++)
	{
		S0[i].m_firstComponent += timeStep * DS[i].m_firstComponent;
		S0[i].m_secondComponent += timeStep * DS[i].m_secondComponent;
		nextPos[i] = S0[i].m_firstComponent;
	}

	for(i = 0; i < CLOTH_SIZE; i++)
	{
		m_clothParticleList[i].m_position = S0[i].m_firstComponent;
		m_clothParticleList[i].m_velocity = S0[i].m_secondComponent;
	}

	ApplyConstrain();
}

void ClothSimulation::IntergrateEulerAndResolveCollistion(float timeStep)
{
	int i = 0;
	hitInfo hit;
	float timeRemaining = timeStep;

	vector<StateSpace> DS(CLOTH_SIZE);
	vector<StateSpace> S0(CLOTH_SIZE);
	vector<Vector3> currPos(CLOTH_SIZE);
	vector<Vector3> nextPos(CLOTH_SIZE);

	while( timeRemaining > 0.f )
	{
		for(i = 0; i < CLOTH_SIZE; i++)
		{
			S0[i].m_firstComponent = m_clothParticleList[i].m_position;
			S0[i].m_secondComponent = m_clothParticleList[i].m_velocity;
			currPos[i] = m_clothParticleList[i].m_position;
		}

		CompuateForceStateSpaceMethod(S0,DS);

		for(i = 0; i < CLOTH_SIZE; i++)
		{
			S0[i].m_firstComponent += timeRemaining * DS[i].m_firstComponent;
			S0[i].m_secondComponent += timeRemaining * DS[i].m_secondComponent;
			nextPos[i] = S0[i].m_firstComponent;
		}

		hit = DetectCollisionWithSpheres(timeRemaining,currPos,nextPos);

		if(hit.index != -1)
		{
			//hack resolve collision
			//m_clothParticleList[hit.index].m_velocity = Vector3(0.f,0.f,0.f);

			//reset S0
			for(i = 0; i < CLOTH_SIZE; i++)
			{
				S0[i].m_firstComponent = m_clothParticleList[i].m_position;
				S0[i].m_secondComponent = m_clothParticleList[i].m_velocity;
			}
			//integrate to collision time
			for(i = 0; i < CLOTH_SIZE; i++)
			{
				S0[i].m_firstComponent += hit.enterExitTime.x * DS[i].m_firstComponent;
				S0[i].m_secondComponent += hit.enterExitTime.x * DS[i].m_secondComponent;
			}

			for(i = 0; i < CLOTH_SIZE; i++)
			{
				m_clothParticleList[i].m_position = S0[i].m_firstComponent;
				m_clothParticleList[i].m_velocity = S0[i].m_secondComponent;
			}

			Vector3 test1 = m_clothParticleList[hit.index].m_velocity;
			CollisionRespone( m_clothParticleList[hit.index], m_sphereList[hit.sphereIndex] );
			Vector3 test2 = m_clothParticleList[hit.index].m_velocity;
			timeRemaining -= hit.enterExitTime.x;
		}
		else
		{
			for(i = 0; i < CLOTH_SIZE; i++)
			{
				m_clothParticleList[i].m_position = S0[i].m_firstComponent;
				m_clothParticleList[i].m_velocity = S0[i].m_secondComponent;
			}
			timeRemaining = 0.f;
		}
	}
}

void ClothSimulation::CollisionRespone( Particle& clothParticle, DebugSphere& sphere )
{
//   	if( clothParticle.m_velocity.CalcLength() < 0.1f)
// 	{
//  		clothParticle.m_velocity = Vector3(0.f,0.f,0.f);
// 		return;
// 	}

	float clothParticleMass = 1.f;
	float sphereMass = 1000.f;
	float dist = ( clothParticle.m_position - sphere.m_center ).CalcLength();
	float totalMass = sphereMass + clothParticleMass;
	
	Vector3 normal = ( clothParticle.m_position - sphere.m_center ) * ( 1 / dist );
	float vCloth = clothParticle.m_velocity.DotProduct( normal );
	float vSphere = sphere.m_velocity.DotProduct( normal );

	float vClothPrime = ( 1.f / totalMass ) * ( ( clothParticleMass * vCloth ) + ( sphereMass * vSphere ) - sphereMass * E * ( vCloth - vSphere ) );
	float vSpherePrime = ( 1.f / totalMass ) * ( ( clothParticleMass * vCloth ) + ( sphereMass * vSphere ) + clothParticleMass * E * ( vCloth - vSphere ) );

	Vector3 vClothF = vClothPrime * normal + ( clothParticle.m_velocity - ( clothParticle.m_velocity.DotProduct(normal) * normal ) ) ;
	Vector3 vSphereF = vSpherePrime * normal + ( sphere.m_velocity - ( sphere.m_velocity.DotProduct(normal) * normal ) ) ;

	//clothParticle.m_velocity = Vector3(10.f,10.f, 100.f);
	
	clothParticle.m_position = clothParticle.m_position - 0.01f * clothParticle.m_velocity;
	clothParticle.m_velocity = vClothF;
	//sphere.m_velocity = vSphereF;

}

void ClothSimulation::IntergrateRK4StateSpaceMethod(float timeStep)
{
	vector<StateSpace> S0(CLOTH_SIZE);
	vector<StateSpace> S1(CLOTH_SIZE);
	vector<StateSpace> S2(CLOTH_SIZE);
	vector<StateSpace> S3(CLOTH_SIZE);
	vector<StateSpace> S4(CLOTH_SIZE);

	vector<StateSpace> DS1(CLOTH_SIZE);
	vector<StateSpace> DS2(CLOTH_SIZE);
	vector<StateSpace> DS3(CLOTH_SIZE);
	vector<StateSpace> DS4(CLOTH_SIZE);

	vector<StateSpace> S_Next(CLOTH_SIZE);

	int i = 0;
	float sixthTimeStep = timeStep / 6.f;

	for(i = 0; i < CLOTH_SIZE; i++)
	{
		S0[i].m_firstComponent = m_clothParticleList[i].m_position;
		S0[i].m_secondComponent = m_clothParticleList[i].m_velocity;
	}

	S1 = S0;
	
	CompuateForceStateSpaceMethod(S1, DS1);

	for( i = 0; i < CLOTH_SIZE; i++)
	{
		S2[i].m_firstComponent = S0[i].m_firstComponent + 0.5f * timeStep * DS1[i].m_firstComponent;
		S2[i].m_secondComponent = S0[i].m_secondComponent + 0.5f * timeStep * DS1[i].m_secondComponent;
	}
	
	CompuateForceStateSpaceMethod(S2, DS2);

	for( i = 0; i < CLOTH_SIZE; i++)
	{
		S3[i].m_firstComponent = S0[i].m_firstComponent + 0.5f * timeStep * DS2[i].m_firstComponent;
		S3[i].m_secondComponent = S0[i].m_secondComponent + 0.5f * timeStep * DS2[i].m_secondComponent;
	}

	CompuateForceStateSpaceMethod(S3, DS3);

	for( i = 0; i < CLOTH_SIZE; i++)
	{
		S4[i].m_firstComponent = S0[i].m_firstComponent + timeStep * DS3[i].m_firstComponent;
		S4[i].m_secondComponent = S0[i].m_secondComponent + timeStep * DS3[i].m_secondComponent;
	}

	CompuateForceStateSpaceMethod(S4, DS4);

	for( i = 0; i < CLOTH_SIZE; i++)
	{
		S_Next[i].m_firstComponent = S0[i].m_firstComponent + sixthTimeStep * ( DS1[i].m_firstComponent + 2 * DS2[i].m_firstComponent + 2 * DS3[i].m_firstComponent + DS4[i].m_firstComponent );
		S_Next[i].m_secondComponent = S0[i].m_secondComponent + sixthTimeStep * ( DS1[i].m_secondComponent + 2 * DS2[i].m_secondComponent + 2 * DS3[i].m_secondComponent + DS4[i].m_secondComponent);
	}

	S0 = S_Next;

	for(i = 0; i < CLOTH_SIZE; i++)
	{
		m_clothParticleList[i].m_position = S0[i].m_firstComponent;
		m_clothParticleList[i].m_velocity = S0[i].m_secondComponent;
	}
}

void ClothSimulation::RenderSphere( Vector3 center ,float r, int lats, int longs )
{
	int i, j;
	glPushMatrix();
	glTranslatef( center.x , center.y, center.z );
	glBegin( GL_QUAD_STRIP );
	for(i = 0; i <= lats; i++) 
	{
		double lat0 = 3.14f * (-0.5 + (double) (i - 1) / lats);
		double z0  = sin(lat0);
		double zr0 =  cos(lat0);

		double lat1 = 3.14f * (-0.5 + (double) i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		for(j = 0; j <= longs; j++) 
		{
			double lng = 2 * 3.14f * (double) (j - 1) / longs;
			double x = r * cos(lng);
			double y = r * sin(lng);

			glNormal3d(x * zr0, y * zr0, z0);
			glVertex3d(x * zr0, y * zr0, z0);
			glNormal3d(x * zr1, y * zr1, z1);
			glVertex3d(x * zr1, y * zr1, z1);
		}
	}
	glEnd();
	glPopMatrix();
}

hitInfo ClothSimulation::DetectCollisionWithSpheres(float timeStep, std::vector<Vector3> currPosList,std::vector<Vector3> nextPosList)
{
	float tEnter = 0.f;
	float tExit = 0.f;
	Vector3 p1;
	Vector3 p2;
	Vector3 q1;
	Vector3 q2;
	Vector3 e;
	Vector3 x0;
	vector<hitInfo> hitInfoList;
	hitInfo temp;
	float particleRadius = 0.08f;

	for(unsigned int i = 0; i < m_clothParticleList.size(); i++)
	{
		p1 = currPosList[i];
		p2 = nextPosList[i];
		for(unsigned int j = 0; j < m_sphereList.size(); j++)
		{
			q1 = m_sphereList[j].m_center;
			q2 = q1;
			e = ( q2 - p2 ) - ( q1 - p1 );
			x0 = q1 - p1;

			float dotee = e.DotProduct(e);
			float dotex0 = e.DotProduct(x0);
			float dotx0x0 = x0.DotProduct(x0);
			float Rsquare = ( particleRadius + m_sphereList[j].m_radius ) * ( particleRadius + m_sphereList[j].m_radius );

			float det = dotex0 * dotex0  - dotee * ( dotx0x0 - Rsquare );

			if(det >= 0.f)
			{
				float detSqrt = sqrt(det);
				float t1 = ( -dotex0 - detSqrt ) / dotee;
				float t2 = ( -dotex0 + detSqrt ) / dotee;
				if( t1 <= 1.f && t2 >= 0.f )
				{
					tEnter = max( t1, 0.f);
					tExit = min( t2, 1.f );
					//Vector2 temp(t1,t2);
					hitInfo temp;
					temp.enterExitTime = Vector2(tEnter,tExit);
					temp.index = i;
					temp.sphereIndex = j;
					hitInfoList.push_back(temp);
				}
				else
					continue;
			}
			else 
				continue;
		}
	}

	if(hitInfoList.size() == 0)
		return temp;

	float smallest = 1000.f;
	int smallestIndex = -1;
	int index;
	for(unsigned int i = 0; i < hitInfoList.size(); i++)
	{
		if( hitInfoList[i].enterExitTime.x < smallest)
		{
			smallest = hitInfoList[i].enterExitTime.x ;
			index = hitInfoList[i].index;
			smallestIndex = i;
		}
	}
	temp.index = index;
	//convert to simulation time
	temp.sphereIndex = hitInfoList[smallestIndex].sphereIndex;
	temp.enterExitTime.x = hitInfoList[smallestIndex].enterExitTime.x * timeStep;
	temp.enterExitTime.y = hitInfoList[smallestIndex].enterExitTime.y * timeStep;

	return temp;
}

void ClothSimulation::ProcessMouseHit()
{
}

void ClothSimulation::ApplyConstrain()
{
	for( unsigned int i = 0; i < ITERATION; i++ )
	{
		for( unsigned int j = 0; j < m_springList.size(); j++ )
		{
			int p1 = m_springList[j].m_endPoint1Index;
			int p2 = m_springList[j].m_endPoint2Index;

			Vector3 direction = m_clothParticleList[p1].m_position - m_clothParticleList[p2].m_position;

			float distance = direction.CalcLength();

			//direction = direction.Normalize();

			float diff = ( distance - m_springList[j].m_restLength )/ distance;

			if( p1 != 0 && p1 != CLOTH_WIDTH_X - 1 )
			{
				m_clothParticleList[p1].m_position -=  ( direction * 0.5f * diff );
			}

			if( p2 != 0 && p2 != CLOTH_WIDTH_X - 1  )
			{
				m_clothParticleList[p2].m_position += ( direction * 0.5f * diff );
			}
		}
	}
}


void ClothSimulation::DeleteConnectionAtPickingPoint()
{
	if( m_pickingIndex1 == m_fixP1 )
		m_fixP1 = -1;
	if( m_pickingIndex1 == m_fixP2 )
		m_fixP2 = -1;
	if( m_pickingIndex1 == m_fixP3 )
		m_fixP3 = -1;
	if( m_pickingIndex1 == m_fixP4 )
		m_fixP4 = -1;
}

void ClothSimulation::ClearFixPoint()
{
	m_fixP1 = -1;
	m_fixP2 = -1;
	m_fixP3 = -1;
	m_fixP4 = -1;
}
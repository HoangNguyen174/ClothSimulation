#include "FlockSimulation.hpp"
#include "GLRender.hpp"


FlockSimulation::FlockSimulation()
{
	Particle temp;
	m_centerPoint = Vector3(0.f,0.f,-3.f);
	temp.m_color = RGBColor(0.f,0.f,1.f,1.f);
	for(unsigned int i = 0; i < FLOCK_SIZE; i++)
	{
		Initialize(temp);
		m_flockList.push_back(temp);
	}

	Particle temp1;
	temp1.m_color = RGBColor(1.f,0.f,0.f,1.f);
	for(unsigned int i = 0; i < PREDATOR_SIZE; i++)
	{
		Initialize(temp1);
		m_predatorList.push_back(temp1);
	}
}

void FlockSimulation::Render()
{
	glEnable(GL_POINT_SMOOTH);
	for(unsigned int i = 0; i < m_flockList.size(); i++)
	{
		RGBColor& color = m_flockList[i].m_color ;
		Vector3& position = m_flockList[i].m_position;
		float& size = m_flockList[i].m_size;

		glColor4f(color.m_red ,color.m_green,color.m_blue,color.m_alpha);
		g_glRender->PointSize(size);
		g_glRender->BeginDraw( GL_POINTS );
		g_glRender->Vertex3f(position.x,position.y,position.z);
		g_glRender->EndDraw();
	}

	for(unsigned int i = 0; i < m_predatorList.size(); i++)
	{
		RGBColor& color = m_predatorList[i].m_color ;
		Vector3& position = m_predatorList[i].m_position;
		float& size = m_predatorList[i].m_size;

		glColor4f(color.m_red ,color.m_green,color.m_blue,color.m_alpha);
		g_glRender->PointSize(size);
		g_glRender->BeginDraw( GL_POINTS );
		g_glRender->Vertex3f(position.x,position.y,position.z);
		g_glRender->EndDraw();
	}

	g_glRender->PointSize(3.f);
	g_glRender->BeginDraw( GL_POINTS );
	glColor4f(1.f,1.f,0.f,1.f);
	g_glRender->Vertex3f(m_centerOfMass.x,m_centerOfMass.y,m_centerOfMass.z);
	g_glRender->EndDraw();
	g_glRender->PointSize(1.f);
}

void FlockSimulation::PullParticleToCenterPoint(Particle& particle)
{
	Vector3 directionToCenter;
	float distToCenter;
	directionToCenter = m_centerPoint - particle.m_position;
	distToCenter = directionToCenter.CalcLength();

	if( distToCenter > MAX_TRAVEL_RADIUS )
	{
		float pullForce = 1.1f;
/*		directionToCenter.Normalize();*/
		particle.m_velocity += directionToCenter.Normalize() * ( ( distToCenter - MAX_TRAVEL_RADIUS ) * pullForce);
	}
}

void FlockSimulation::ComputeAndApplyForceModelToFlock(Particle& particle, int index)
{
	float zoneRadius = 1.f;
	float thresh = 0.1f; 
	float highThres = 0.8f;

	for(unsigned int j = 0; j < m_flockList.size(); j++)
	{
		if(j != index )
		{
			Vector3 direction = m_flockList[index].m_position - m_flockList[j].m_position;
			float distSquare = direction.CalcLengthSquare();

			if(distSquare <= zoneRadius * zoneRadius)
			{
				float percent = distSquare/ (zoneRadius * zoneRadius);
				if(percent < thresh)
				{
					float F = (thresh / percent - 1.f) * 0.01f;
/*					direction.Normalize();*/
					direction = direction.Normalize() * F;
					m_flockList[index].m_acceleration += direction;
					m_flockList[j].m_acceleration -= direction;
				}
				else if(percent < highThres)
				{
					float threshDela = highThres - thresh;
					float adjustPercent = ( percent - thresh)/threshDela;
					float F = (.5f - cos(adjustPercent * 3.14f * 2.f) * 0.5f + 0.5f ) * 0.01f;
// 					m_flockList[index].m_velocity.Normalize();
// 					m_flockList[j].m_velocity.Normalize();

					m_flockList[index].m_acceleration += m_flockList[j].m_velocity.Normalize() * F;
					m_flockList[j].m_acceleration += m_flockList[index].m_velocity.Normalize() * F;
				}
				else
				{
					float threshDelta = 1.0f - highThres;
					float adjustPercent = (percent - highThres)/threshDelta;
					float F = (.5f - cos(adjustPercent * 3.14f * 2.f) * 0.5f + 0.5f ) * 0.01f;
/*					direction.Normalize();*/
					direction = direction.Normalize() * F;
					m_flockList[index].m_acceleration -= direction;
					m_flockList[j].m_acceleration += direction;
				}
			}
		}
	}

	for(unsigned int i = 0; i < m_predatorList.size(); i++)
	{
		Vector3 direction = m_flockList[index].m_position - m_predatorList[i].m_position;
		float distSquare = direction.CalcLengthSquare();

		if( distSquare < PREDATOR_RADIUS * PREDATOR_RADIUS)
		{
			if(distSquare > 0.1f)
			{
				float F = ( (PREDATOR_RADIUS * PREDATOR_RADIUS )/distSquare - 1.0f ) * .1f;
				//direction.Normalize();
				direction = direction.Normalize() * F;
				m_flockList[index].m_acceleration += direction;
			}
			else
			{
				m_flockList.erase(m_flockList.begin() + index);
				m_predatorList[i].m_size += 0.3f;
				break;
			}
		}
	}
}

void FlockSimulation::ComputeAndApplyForceModelToPredator(Particle& particle, int index)
{
	Vector3 direction = m_flockList[index].m_position - m_centerOfMass;
	float dist = direction.CalcLength();
	
	if(dist < 0.1f)
	{
		direction = direction.Normalize();
		particle.m_acceleration += direction ;
	}
}


void FlockSimulation::ConstrainVelocity(Particle& particle)
{
	float velSquare = particle.m_velocity.CalcLengthSquare();
	if(velSquare > MAX_SPEED * MAX_SPEED)
	{
	/*	particle.m_velocity.Normalize();*/
		particle.m_velocity = particle.m_velocity.Normalize() * MAX_SPEED;
	}
	else if( velSquare < MIN_SPEED * MIN_SPEED )
	{
	/*	particle.m_velocity.Normalize();*/
		particle.m_velocity = particle.m_velocity.Normalize() * MIN_SPEED;
	}
}

void FlockSimulation::Update(float elapsedTime)
{
	for(unsigned int i = 0; i < m_flockList.size(); i++)
	{
		ComputeAndApplyForceModelToFlock(m_flockList[i],i);
		m_flockList[i].m_velocity += m_flockList[i].m_acceleration * elapsedTime;
		m_flockList[i].m_position += m_flockList[i].m_velocity * elapsedTime;
		PullParticleToCenterPoint(m_flockList[i]);
		ConstrainVelocity(m_flockList[i]);
	}

	m_centerOfMass = ComputeCenterOfMass();

	for(unsigned int j = 0; j < m_predatorList.size(); j++)
	{
		ComputeAndApplyForceModelToPredator(m_predatorList[j],j);
		m_predatorList[j].m_position += m_predatorList[j].m_velocity * elapsedTime;
		m_predatorList[j].m_velocity += m_predatorList[j].m_acceleration * elapsedTime;
		PullParticleToCenterPoint(m_predatorList[j]);
		ConstrainVelocity(m_predatorList[j]);
	}
}

void FlockSimulation::Initialize(Particle& particle)
{	
		boost::random::uniform_real_distribution<> randomPolar( 0, 90);
		boost::random::uniform_real_distribution<> randomAzimuthal( 1.0, 90.0);
		boost::random::uniform_real_distribution<> randomRadius( 0.5, 5.0);
		boost::random::uniform_real_distribution<> randomLifeSpan( 2.0, 4.0);
		boost::random::uniform_real_distribution<> randomSize( 1, 5 );

		boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomPolar( randomGenerator, randomPolar );
		boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomAzimuthal( randomGenerator, randomAzimuthal );
		boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomRadius( randomGenerator, randomRadius );
		boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomLifeSpan( randomGenerator, randomLifeSpan );
		boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution<> > generateRandomSize(randomGenerator, randomSize);

		particle.m_lifeSpanInSecond = (float)generateRandomLifeSpan();
		
		float polar = (float)generateRandomPolar();
		float azimuthal = (float)generateRandomAzimuthal();
		float radius = (float)generateRandomRadius();
		float size = (float)generateRandomSize();
		particle.m_size = size;
		particle.m_mass = 1.f;
		particle.m_velocity = Vector3::ConvertFromSphericalCoordToCartersianCoord(radius,azimuthal,polar) ;
		particle.m_position = Vector3::ConvertFromSphericalCoordToCartersianCoord(radius,azimuthal,polar) ;
}

Vector3 FlockSimulation::ComputeCenterOfMass()
{
	float massTotal = 0.f;
	Vector3 mr;
	for(unsigned int i = 0; i < m_flockList.size(); i++)
	{
		massTotal += m_flockList[i].m_mass;
		mr += m_flockList[i].m_mass * m_flockList[i].m_position;
	}
	return ( mr * (1 / massTotal) );
}
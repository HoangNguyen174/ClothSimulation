#include "ParticleSystem.hpp"

void ParticleSystem::Initialize()
{
	m_particleList.reserve(MAX_NUMBER_PARTICLE);
	Particle temp;
	if(m_isSystemLiveForever)
		temp.m_isLiveForever = true;
	for(unsigned int i = 0; i < MAX_NUMBER_PARTICLE; i++)
	{
		m_emitterStrategy->Initialize(temp);
		m_particleList.push_back(temp);
	}
}

void ParticleSystem::Update(float elapsedTime)
{
	for(unsigned int i = 0; i < m_particleList.size(); i++)
	{
		Particle& particle = m_particleList[i];
		if(!particle.m_isLiveForever)
		{
			if(particle.m_lifeSpanInSecond > 0)
				particle.m_lifeSpanInSecond -= elapsedTime;

			if(particle.m_lifeSpanInSecond < 0)
			{
				if(!m_discreteUpdate)
					m_emitterStrategy->Initialize(particle);
				else
					m_particleList.erase( m_particleList.begin() + i );
			}
		}
	}

	for(unsigned int i = 0; i < m_particleList.size(); i++)
	{
		Particle& particle = m_particleList[i];
		m_integrationMethod->Integrate(&particle.m_position, &particle.m_velocity,elapsedTime);
	}

	if(m_particleList.size() <= 0)
		Initialize();
}

void ParticleSystem::Render()
{
	//glPushMatrix();
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	for(unsigned int i = 0; i < m_particleList.size(); i++)
	{
		RGBColor& color = m_particleList[i].m_color ;
		Vector3& position = m_particleList[i].m_position;
		float& size = m_particleList[i].m_size;

		glColor4f(color.m_red ,color.m_green,color.m_blue,color.m_alpha * m_particleList[i].m_lifeSpanInSecond/4.f);
		g_glRender->PointSize(size);
		g_glRender->BeginDraw( GL_POINTS );
		g_glRender->Vertex3f(position.x,position.y,position.z);
		g_glRender->EndDraw();
	}
	g_glRender->PointSize(1.f);
	glDisable(GL_BLEND);
	//glPopMatrix();
}

void ParticleSystem::RenderParticle()
{
// 	int i, j;
// 	int lats = 20;
// 	int longs = 20;
// 	for(i = 0; i <= lats; i++) 
// 	{
// 		double lat0 = 3.14f * (-0.5 + (double) (i - 1) / lats);
// 		double z0  = m_particleSize * sin(lat0);
// 		double zr0 = m_particleSize * cos(lat0);
// 
// 		double lat1 = 3.14f * (-0.5 + (double) i / lats);
// 		double z1 =  m_particleSize * sin(lat1);
// 		double zr1 = m_particleSize * cos(lat1);
// 
// 
// 		g_glRender->BeginDraw( GL_QUAD_STRIP );
// 		for(j = 0; j <= longs; j++) 
// 		{
// 			double lng = 2 * 3.14f * (double) (j - 1) / longs;
// 			double x = cos(lng);
// 			double y = sin(lng);
// 
// 			glNormal3d(x * zr0, y * zr0, z0);
// 			//glTexCoord2d( x * zr0, y * zr0 );
// 			glVertex3d(x * zr0, y * zr0, z0);
// 
// 			//glNormal3d(x * zr1, y * zr1, z1);
// 			//glTexCoord2d( x * zr1, y * zr1 );
// 			glVertex3d(x * zr1, y * zr1, z1);
// 		}
// 		g_glRender->EndDraw();
// 	}
// 		g_glRender->BeginDraw( GL_POINTS );
// 		g_glRender->Vertex3f()
// 		g_glRender->EndDraw();
}
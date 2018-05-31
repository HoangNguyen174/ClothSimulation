#ifndef RGBCOLOR_H
#define RGBCOLOR_H

#include "MathUtilities.hpp"

class RGBColor
{
	public: 
		float m_red;
		float m_green;
		float m_blue;
		float m_alpha;

	public:
		RGBColor()
		{
			m_red = 0.f;
			m_green = 0.f;
			m_blue = 0.f;
			m_alpha = 0.f;
		};
		RGBColor(float red, float green, float blue, float alpha)
		{
			m_red =  MathUtilities::Clamp(0.f,1.f,red);
			m_green = MathUtilities::Clamp(0.f,1.f,green);
			m_blue = MathUtilities::Clamp(0.f,1.f,blue);
			m_alpha = MathUtilities::Clamp(0.f,1.f,alpha);
		};
		~RGBColor() {};
		void SetColor(float red,float green, float blue, float alpha)
		{
			m_red =  MathUtilities::Clamp(0.f,1.f,red);
			m_green = MathUtilities::Clamp(0.f,1.f,green);
			m_blue = MathUtilities::Clamp(0.f,1.f,blue);
			m_alpha = MathUtilities::Clamp(0.f,1.f,alpha);
		};
		

};

#endif
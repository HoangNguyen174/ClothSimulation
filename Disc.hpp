#ifndef DISC_H
#define DISC_H
#include "Vector2.hpp"
#include "MathUtilities.hpp"
using namespace std;
class Disc
{
	private:
		float m_radius;
		Vector2 m_center;
	public:
		Disc();
		Disc(Vector2 center,float radius);
		//Disc(const AABB2& boudningRectangle);
		~Disc();
		bool IsPointInside(const Vector2& point) const;
		bool IsOverlappingWithDisc(const Disc& otherDisc) const;
		float CalcDistanceFromDisc(const Disc& otherDisc) const;
		float CaclcDistanceFromPoint(const Vector2& point) const;
		void SetCenterAndRadius(Vector2 center,float radius);
		Vector2 GetCenter();
		float GetRadius();
		void SetCenter(Vector2 center);
		void SetRadius(float radius);
		
};
#endif
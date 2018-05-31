#include "Disc.hpp"

Disc::Disc()
{
}

Disc::Disc(Vector2 center, float radius)
{
	m_center = center;
	m_radius = radius;
}

Disc::~Disc()
{
}

void Disc::SetCenterAndRadius(Vector2 center,float radius)
{
	m_radius = radius;
	m_center = center;
}

Vector2 Disc::GetCenter()
{
	return m_center;
}

float Disc::GetRadius()
{
	return m_radius;
}

void Disc::SetCenter(Vector2 center)
{
	m_center = center;
}

void Disc::SetRadius(float radius)
{
	m_radius = radius;
}

bool Disc::IsOverlappingWithDisc(const Disc& otherDisc) const
{
	float distance = this->CalcDistanceFromDisc(otherDisc);
	if((m_radius + otherDisc.m_radius) >= distance)
		return true;
	else
		return false;
}

float Disc::CalcDistanceFromDisc(const Disc& otherDisc) const
{
    Vector2 DiscDistanceFromCenter;
	Vector2 otherCenter = otherDisc.m_center;
	DiscDistanceFromCenter = otherCenter - m_center;

	return DiscDistanceFromCenter.CalcLength();
}

float Disc::CaclcDistanceFromPoint(const Vector2& point) const
{
    Vector2 DiscDistanceFromCenter;
	Vector2 tempPoint = point;
	DiscDistanceFromCenter = tempPoint - m_center;

	return DiscDistanceFromCenter.CalcLength();
}

bool Disc::IsPointInside(const Vector2& point) const
{
	if(this->CaclcDistanceFromPoint(point) < m_radius)
		return true;
	else
		return false;
}
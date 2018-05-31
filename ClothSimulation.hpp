#ifndef CLOTH_SIMULATION_H
#define CLOTH_SIMULATION_H
#include "Particle.hpp"
#include <vector>
#include "Vector3.h"
#include "vector2.hpp"
#include "Texture.hpp"
#include "MathUtilities.hpp"
#include "DebugSphere.hpp"

const int CLOTH_WIDTH_X = 10;
const int CLOTH_HEIGHT_Y = 10;
const int CLOTH_SIZE = CLOTH_HEIGHT_Y * CLOTH_WIDTH_X;
const float DIST_BETWEEN_POINT = .3f;
const float KS_STRUCT = 7.5f;
const float KD_STRUCT = -1.25f;
const float KS_SHEAR = 7.5f;
const float KD_SHEAR = -1.25f;
const float KS_BEND = 7.85f;
const float KD_BEND = -1.25f;
const Vector3 GRAVITY = Vector3( 0.f,0.f,-0.098f);
const float DAMPING = -0.25f;
const int ITERATION = 5;
const float E = 0.0f;
const float INITIAL_HEIGHT = 3.f;
enum StringType { STRUCTURAL_SPRING = 0, SHEAR_SPRING, BEND_SPRING };

class Spring
{
	public:
		int m_endPoint1Index;
		int m_endPoint2Index;
		float m_restLength;
		float m_ks;
		float m_kd;
		StringType m_type;
};

struct StateSpace
{
	Vector3 m_firstComponent;
	Vector3 m_secondComponent;
};

struct hitInfo
{
	hitInfo()
	{
		enterExitTime = Vector2(-1.f,-1.f);
		index = -1;
	}
	Vector2 enterExitTime;
	int index;
	int sphereIndex;
};

class ClothSimulation
{
	public:
		std::vector<Particle> m_clothParticleList;
		std::vector<Spring> m_springList;
		std::vector<Vector3> m_forceOnClothPartcleList;
		std::vector<DebugSphere> m_sphereList;
		Vector3 m_windForce;
		Texture* m_texture;
		float m_texWidth;
		float m_texHeight;
		int m_pickingIndex1;
		int m_pickingIndex2;
		Vector3 m_forceOnPicking;
		int m_fixP1;
		int m_fixP2;
		int m_fixP3;
		int m_fixP4;

	public:
		ClothSimulation();
		void Update(float elapsedTime);
		void Render();
		void ProcessMouseHit();
		void ClearFixPoint();

	private:
		void Intialize();
		void ComputeForce();
		void AddSpring(int endPoint1, int endPoint2, float ks, float kd, StringType type);
		void IntergrateEuler(Vector3& pos, Vector3& vel, int index, float timeStep);
		void IntergrateVerlet(float timeStep);
		void IntergrateRK4(float timeStep);
		Vector2 ConvertIndexTo2DIndex(int index);
		void CompuateForceStateSpaceMethod(const std::vector<StateSpace>& state, std::vector<StateSpace>& stateDev);
		void IntergrateRK4StateSpaceMethod(float timeStep);
		void IntergrateEulerStateSpadeMethod(float timeStep);
		hitInfo DetectCollisionWithSpheres(float timeStep, std::vector<Vector3> currPosList,std::vector<Vector3> nextPosList);
		void RenderSphere( Vector3 center, float r, int lats, int longs);
		void IntergrateEulerAndResolveCollistion(float timeStep);
		void ApplyConstrain();
		void ComputeExternalForce();
		void CollisionRespone( Particle& clothParticle, DebugSphere& sphere);
		void DeleteConnectionAtPickingPoint();

};

#endif
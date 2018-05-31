#ifndef WORLD_H
#define WORLD_H
#include "GameCommon.hpp"
#include "GLRender.hpp"
#include "DebugObject.hpp"
#include "DebugPoint.hpp"
#include "DebugLine.hpp"
#include "DebugArrow.hpp"
#include "DebugAABB3.hpp"
#include "DebugSphere.hpp"
#include "ParticleSystem.hpp"
#include "IntegrationMethodStrategy.hpp"
#include "ForwardEulerIntegrationStrategy.hpp"
#include "ForceModelStrategy.hpp"
#include "EmitterStrategy.hpp"
#include "ExplosionEmitterStrategy.hpp"
#include "GravitationalForceModel.hpp"
#include "SmokeEmitterStrategy.hpp"
#include "WindForceModel.hpp"
#include "LorenzSystemModel.hpp"
#include "FlockSimulation.hpp"
#include "ClothSimulation.hpp"
#include "Matrix44.hpp"
#include "MatrixStack.hpp"

class World
{
	public:
		Camera3D m_camera3D;
		ClothSimulation* m_clothSimulation;
		MatrixStack m_matrixStack;
		Matrix44 m_projectionMatrix;
		Matrix44 m_viewMatrix;
		bool m_leftMouseDown;

	public:
		World();
		~World();
		void Initialize();
		bool ProcessKeyDownEvent(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam );
		bool ProcessKeyUpEnvent(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam );
		void SetupPerspectiveProjection();
		void ApplyCameraTransform(Camera3D camera);
		void UpdateCameraFromKeyboardAndMouse(Camera3D& m_camera3D,float elapsedTime);
		Vector2 GetMouseSinceLastChecked();
		void Update(float elapsedTime);
		void Render();
		void RenderWorldAxes();
		int GetClothParticleIndexFromMousePicking();
		bool InvertMatrix(const float m[16], float outMatrix[16]);
		int GetSphereMouseRayIntersection(const Vector3& mouseRayDirection );
		

};

extern World* theWorld;
extern bool IsKeyDownKeyboard[256];
extern bool IsKeyDownLastFrame[256];

#endif
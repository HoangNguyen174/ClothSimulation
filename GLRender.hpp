#ifndef GL_MYRENDER
#define GL_MYRENDER
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "GameCommon.hpp"
#include "DebugObject.hpp"
//#include "DebugAABB3.hpp"
//#include "DebugArrow.hpp"
//#include "DebugLine.hpp"
//#include "DebugPoint.hpp"
//#include "DebugSphere.hpp"

class GLRender
{
	public:
		std::vector<DebugObject*> m_debugPrimitiveToDraw;

	public:
		GLRender();
		~GLRender();
		void BeginDraw(int value);
		void Clear(int mode);
		void EndDraw();
		void Enable(int mode);
		void Disable(int mode);
		void BlendFunc(int mode, int mode1);
		void PushMatrix();
		void PopMatrix();
		void LoadIdentityMatrix();
		void Translatef(float x, float y, float z);
		void Translated(double x, double y, double z);
		void Rotatef(float angle, float x, float y, float z);
		void Rotated(double angle, double x, double y, double z);
		void Scalef(float x, float y, float z);
		void Scaled(double x, double y, double z);
		void BindTexture(int mode, int id);
		void Vertex3f(float x,float y, float z);
		void Vertex3d(double x, double y, double z);
		void Vertex3i(int x,int y,int z);
		void TexCoord2d(double x, double y);
		void TexCoord2f(float x, float y);
		void Color4f(float r, float g, float b, float alpha);
		void Color3f(float r, float g, float b);
		void ClearColor(float r, float g, float b, float alpha);
		void ClearDepth(double value);
		void EnableDepthMask();
		void DisableDepthMask();
		void DeleteBuffers(int size, unsigned int* id);
		void EnableClientState(int mode);
		void DisableClientState(int mode);
		void BindBuffer(int mode, unsigned int id);
		void DrawArray(int mode,int firstIndex, int size);
		void GenerateBuffer(int size, unsigned int* id);
		void PolygonMode(int mode, int mode1);
		void PointSize(float size);
		void VertexPointer(int, int, int,const GLvoid*);
		void ColorPointer(int, int, int,const GLvoid*);
		void TexCoordPointer(int, int, int, const GLvoid*);
		void BufferData(int mode, GLsizeiptr size, const GLvoid*, int mode1 );
		void FrontFace(int mode);
		void LineWidth(float size);
};

extern GLRender* g_glRender;
#endif
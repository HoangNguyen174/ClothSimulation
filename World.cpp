#include "World.hpp"

World* theWorld = nullptr;
bool IsKeyDownKeyboard[256];
bool IsKeyDownLastFrame[256];

World::World()
{
	Initialize();
}

World::~World()
{
}

void World::Initialize()
{
	for(int i = 0; i < 256; i++)
	{
		IsKeyDownKeyboard[i] = false;
		IsKeyDownLastFrame[i] = false;
	}
	m_camera3D.m_cameraPosition = Vector3(-5.f,0.f,1.f);
	m_leftMouseDown = false;
	m_clothSimulation = new ClothSimulation();
}

void World::SetupPerspectiveProjection()
{
	float aspect = (16.f/9.f);
	float fovX = 70.f;
	float fovY = (fovX / aspect);
	float zNear = 0.1f;
	float zFar = 1000.f;
// 	g_glRender->LoadIdentityMatrix();
// 	gluPerspective(fovY,aspect,zNear,zFar);
	m_matrixStack.LoadIdentity();
	m_matrixStack.PushMatrix(Matrix44::CreatePerspectiveMatrix(fovY,aspect,zNear,zFar));
	m_projectionMatrix = Matrix44::CreatePerspectiveMatrix(fovY,aspect,zNear,zFar);
}

void World::ApplyCameraTransform(Camera3D camera)
{
// 	g_glRender->Rotatef(-90.f,1.f,0.f,0.f);
// 	g_glRender->Rotatef(90.f, 0.f,0.f,1.f);
// 
// 	g_glRender->Rotatef(-camera.m_rollDegreesAboutX, 1.f,0.f,0.f);
// 	g_glRender->Rotatef(-camera.m_pitchDegreesAboutY, 0.f,1.f,0.f);
// 	g_glRender->Rotatef(-camera.m_yawDegreesAboutZ , 0.f,0.f,1.f);
// 
// 	g_glRender->Translatef(-camera.m_cameraPosition.x, -camera.m_cameraPosition.y, -camera.m_cameraPosition.z);

	m_matrixStack.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3(1.f,0.f,0.f), -90.f ) );
	m_matrixStack.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3(0.f,0.f,1.f),  90.f ) );

	m_matrixStack.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3(1.f,0.f,0.f), -camera.m_rollDegreesAboutX ) );
	m_matrixStack.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3(0.f,1.f,0.f), -camera.m_pitchDegreesAboutY ) );
	m_matrixStack.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3(0.f,0.f,1.f), -camera.m_yawDegreesAboutZ ) );

	m_matrixStack.PushMatrix( Matrix44::CreateTranslationMatrix( Vector3 ( -camera.m_cameraPosition.x, -camera.m_cameraPosition.y, -camera.m_cameraPosition.z )));

	MatrixStack temp;
	temp.LoadIdentity();
	temp.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3(1.f,0.f,0.f), -90.f ) );
	temp.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3(0.f,0.f,1.f),  90.f ) );
	temp.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3(1.f,0.f,0.f), -camera.m_rollDegreesAboutX ) );
	temp.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3(0.f,1.f,0.f), -camera.m_pitchDegreesAboutY ) );
	temp.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3(0.f,0.f,1.f), -camera.m_yawDegreesAboutZ ) );
	temp.PushMatrix( Matrix44::CreateTranslationMatrix( Vector3 ( -camera.m_cameraPosition.x, -camera.m_cameraPosition.y, -camera.m_cameraPosition.z )));

	m_viewMatrix = temp.StackTop();
	
	glLoadMatrixf( m_matrixStack.StackTop() );
}

bool World::ProcessKeyDownEvent(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	unsigned char asKey = (unsigned char) wParam;
	switch( wmMessageCode )
	{
		case WM_KEYDOWN:
			IsKeyDownKeyboard[asKey] = true;
			return true;
			break;

		case WM_KEYUP:
			IsKeyDownKeyboard[asKey] = false;
			return true;
			break;

		case WM_LBUTTONDOWN:
			m_leftMouseDown = true;	
			break;

		case WM_LBUTTONUP:
			m_leftMouseDown = false;	
			break;
	}
	return true;
}

Vector2 World::GetMouseSinceLastChecked()
{
	POINT centerCursorPos = { 800, 450 };
	POINT cursorPos;
	GetCursorPos( &cursorPos );
	SetCursorPos( centerCursorPos.x, centerCursorPos.y );
	Vector2i mouseDeltaInts( cursorPos.x - centerCursorPos.x, cursorPos.y - centerCursorPos.y );
	Vector2 mouseDeltas( (float) mouseDeltaInts.x, (float) mouseDeltaInts.y );
	return mouseDeltas;
}

void World::UpdateCameraFromKeyboardAndMouse(Camera3D& camera, float elapsedTime)
{
	const float degreesPerMouseDelta = 0.02f;
	Vector2 mouseDeltas = GetMouseSinceLastChecked();
	camera.m_yawDegreesAboutZ -= (degreesPerMouseDelta * mouseDeltas.x);

	camera.m_pitchDegreesAboutY += (degreesPerMouseDelta * mouseDeltas.y);
	camera.m_pitchDegreesAboutY = MathUtilities::Clamp(-90.f,90.f,camera.m_pitchDegreesAboutY);

	float cameraYawRadians = MathUtilities::DegToRad(camera.m_yawDegreesAboutZ);
	Vector3 cameraForwardXYVector( cos(cameraYawRadians), sin( cameraYawRadians ),0.f);

	Vector3 movementVector( 0.f,0.f,0.f );

	if( IsKeyDownKeyboard[ 'W' ] )
		movementVector += cameraForwardXYVector * 2.f;

	if( IsKeyDownKeyboard[ 'A' ] )
		movementVector += Vector3(-cameraForwardXYVector.y,cameraForwardXYVector.x,0.f) * 2.f;

	if( IsKeyDownKeyboard[ 'D' ] )
		movementVector += Vector3(cameraForwardXYVector.y,-cameraForwardXYVector.x,0.f) * 2.f;

	if( IsKeyDownKeyboard[ 'S' ] )
		movementVector += Vector3(-cameraForwardXYVector.x,-cameraForwardXYVector.y,0.f) * 2.f;

	if( IsKeyDownKeyboard[ 'E' ] )
		movementVector += Vector3(0.f,0.f,2.f);

	if( IsKeyDownKeyboard[ 'C' ] )
		movementVector += Vector3(0.f,0.f,-2.f);

	if( IsKeyDownKeyboard[ 'T' ])
		m_clothSimulation->ClearFixPoint();
	
	if( IsKeyDownKeyboard[ VK_UP ])
	{
		float x = (float)MathUtilities::GetRandomNumber( -1, 1 ) / 10.f;
		float y = (float)MathUtilities::GetRandomNumber( -1, 1 ) / 10.f;
		float z = (float)MathUtilities::GetRandomNumber( -1, 1 ) / 10.f;
		m_clothSimulation->m_windForce = Vector3(x,y,z);
	}

	if( IsKeyDownKeyboard[ VK_DOWN ])
	{
		m_clothSimulation->m_windForce = Vector3(0.f,0.f,0.f);
	}


	if( IsKeyDownKeyboard[ 'R' ])
	{
		m_clothSimulation = nullptr;
		delete m_clothSimulation;
		m_clothSimulation = new ClothSimulation();
	}

	camera.m_cameraPosition += movementVector * elapsedTime;
}

void World::Update(float elapsedTime)
{
	if( !IsKeyDownKeyboard[ VK_CONTROL ])
	{
		HCURSOR cursor = LoadCursor( NULL,NULL);
		SetCursor( cursor );
		UpdateCameraFromKeyboardAndMouse(m_camera3D,elapsedTime);
	}
	else
	{
		HCURSOR cursor = LoadCursor( NULL,IDC_ARROW );
		SetCursor( cursor );
		if( m_leftMouseDown )
		{
			m_clothSimulation->m_pickingIndex1 = GetClothParticleIndexFromMousePicking();
			if( m_clothSimulation->m_pickingIndex1 != -1 )
			{
				DebuggerPrintf( "Hit particle %i.\n", m_clothSimulation->m_pickingIndex1);
			}
			else
				DebuggerPrintf( "Hit Air.\n");
			m_leftMouseDown = false;
		}
	}

	m_clothSimulation->ProcessMouseHit();
	m_clothSimulation->Update(elapsedTime);
}

void World::Render()
{
	g_glRender->ClearColor( 0.3f,0.5f,1.f,1.f );
	g_glRender->ClearDepth( 1.f );
	g_glRender->Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	g_glRender->EnableDepthMask();
	g_glRender->Enable(GL_DEPTH_TEST);
	SetupPerspectiveProjection();
	ApplyCameraTransform( m_camera3D );
	RenderWorldAxes();
	m_clothSimulation->Render();
}

void World::RenderWorldAxes()
{
	g_glRender->PushMatrix();
	g_glRender->Disable(GL_DEPTH_TEST);
	g_glRender->Scalef(2.f,2.f,2.f);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Color4f(1.f,0.f,0.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(1.f,0.f,0.f);

		g_glRender->Color4f(0.f,1.f,0.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(0.f,1.f,0.f);

		g_glRender->Color4f(0.f,0.f,1.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(0.f,0.f,1.f);
	}
	g_glRender->EndDraw();
	
	g_glRender->Enable(GL_DEPTH_TEST);
	g_glRender->LineWidth(3.f);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Color4f(1.f,0.f,0.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(1.f,0.f,0.f);

		g_glRender->Color4f(0.f,1.f,0.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(0.f,1.f,0.f);

		g_glRender->Color4f(0.f,0.f,1.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(0.f,0.f,1.f);
	}
	g_glRender->EndDraw();

	g_glRender->PopMatrix();
}

int World::GetClothParticleIndexFromMousePicking()
{ 
	POINT cursorPos;
	GetCursorPos( &cursorPos );
	
	//convert into normalized device Coordinates
	float x = ( 2.f * cursorPos.x ) / 1600 - 1.f;
	float y = 1.f - ( 2.f * cursorPos.y ) / 900;
	float z = 1.f;
	Vector3 mouseRayInNDS = Vector3(x,y,z);

	Vector3 mouseRayInClip( x,y,-1.f );

	Matrix44 inverseOfProjectionMatrix;

	InvertMatrix( m_projectionMatrix.m_matrix, inverseOfProjectionMatrix.m_matrix );

	Vector3 mouseRayInCameraSpace = inverseOfProjectionMatrix.TransformPoint( mouseRayInClip );

	mouseRayInCameraSpace.z = -1.f;

	Matrix44 inverseViewMatrix;
	
	InvertMatrix( m_viewMatrix.m_matrix, inverseViewMatrix.m_matrix );

	Vector3 mouseRayInWorldSpace = inverseViewMatrix.TransformDirection( mouseRayInCameraSpace );

	//mouseRayInWorldSpace = mouseRayInCameraSpace.Normalize();

	return GetSphereMouseRayIntersection( mouseRayInWorldSpace );
}

int World::GetSphereMouseRayIntersection( const Vector3& rayDirection )
{
	Vector3 rayOrigin = m_camera3D.m_cameraPosition;
	Vector3 rayDir = rayDirection;
	std::vector<int> hitList;
	float sRadiusSquare = 0.08f * 0.08f;
	for( unsigned int i = 0; i < m_clothSimulation->m_clothParticleList.size(); i++)
	{
		Vector3 sCenter = m_clothSimulation->m_clothParticleList[i].m_position; 
		float a = rayDir.CalcLength();
		float b = 2 * ( rayOrigin - sCenter ).DotProduct(rayDir);
		float c = ( rayOrigin - sCenter ).DotProduct( rayOrigin - sCenter ) - sRadiusSquare;
		float d = b * b - 4 * a * c;

		if( d >= 0)
		   hitList.push_back(i);
	}

	float smallest = 10000000.f;
	int smallestIndex = -1.f;
	for( unsigned int i = 0; i < hitList.size();i++)
	{
		int index = hitList[i];
		Vector3 pos =  m_clothSimulation->m_clothParticleList[index].m_position;
		float dist = (pos - rayOrigin).CalcLength();
		if( dist < smallest)
		{
			smallest = dist;
			smallestIndex = index;
		}
	}

	return smallestIndex;
}

bool World::InvertMatrix(const float m[16], float invOut[16])
{
	double inv[16], det;
	int i;

	inv[0] = m[5]  * m[10] * m[15] - 
			 m[5]  * m[11] * m[14] - 
			 m[9]  * m[6]  * m[15] + 
			 m[9]  * m[7]  * m[14] +
			 m[13] * m[6]  * m[11] - 
			 m[13] * m[7]  * m[10];

	inv[4] = -m[4]  * m[10] * m[15] + 
			  m[4]  * m[11] * m[14] + 
			  m[8]  * m[6]  * m[15] - 
			  m[8]  * m[7]  * m[14] - 
			  m[12] * m[6]  * m[11] + 
			  m[12] * m[7]  * m[10];

	inv[8] = m[4]  * m[9] * m[15] - 
			 m[4]  * m[11] * m[13] - 
			 m[8]  * m[5] * m[15] + 
			 m[8]  * m[7] * m[13] + 
			 m[12] * m[5] * m[11] - 
			 m[12] * m[7] * m[9];

	inv[12] = -m[4]  * m[9] * m[14] + 
			   m[4]  * m[10] * m[13] +
			   m[8]  * m[5] * m[14] - 
			   m[8]  * m[6] * m[13] - 
			   m[12] * m[5] * m[10] + 
			   m[12] * m[6] * m[9];

	inv[1] = -m[1]  * m[10] * m[15] + 
			  m[1]  * m[11] * m[14] + 
			  m[9]  * m[2] * m[15] - 
			  m[9]  * m[3] * m[14] - 
			  m[13] * m[2] * m[11] + 
			  m[13] * m[3] * m[10];

	inv[5] = m[0]  * m[10] * m[15] - 
			 m[0]  * m[11] * m[14] - 
			 m[8]  * m[2] * m[15] + 
			 m[8]  * m[3] * m[14] + 
			 m[12] * m[2] * m[11] - 
			 m[12] * m[3] * m[10];

	inv[9] = -m[0]  * m[9] * m[15] + 
			  m[0]  * m[11] * m[13] + 
			  m[8]  * m[1] * m[15] - 
			  m[8]  * m[3] * m[13] - 
			  m[12] * m[1] * m[11] + 
			  m[12] * m[3] * m[9];

	inv[13] = m[0]  * m[9] * m[14] - 
			  m[0]  * m[10] * m[13] - 
			  m[8]  * m[1] * m[14] + 
			  m[8]  * m[2] * m[13] + 
			  m[12] * m[1] * m[10] - 
			  m[12] * m[2] * m[9];

	inv[2] = m[1]  * m[6] * m[15] - 
			 m[1]  * m[7] * m[14] - 
			 m[5]  * m[2] * m[15] + 
			 m[5]  * m[3] * m[14] + 
			 m[13] * m[2] * m[7] - 
			 m[13] * m[3] * m[6];

	inv[6] = -m[0]  * m[6] * m[15] + 
			  m[0]  * m[7] * m[14] + 
			  m[4]  * m[2] * m[15] - 
			  m[4]  * m[3] * m[14] - 
			  m[12] * m[2] * m[7] + 
			  m[12] * m[3] * m[6];

	inv[10] = m[0]  * m[5] * m[15] - 
			  m[0]  * m[7] * m[13] - 
			  m[4]  * m[1] * m[15] + 
			  m[4]  * m[3] * m[13] + 
			  m[12] * m[1] * m[7] - 
			  m[12] * m[3] * m[5];

	inv[14] = -m[0]  * m[5] * m[14] + 
			   m[0]  * m[6] * m[13] + 
			   m[4]  * m[1] * m[14] - 
			   m[4]  * m[2] * m[13] - 
			   m[12] * m[1] * m[6] + 
			   m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] + 
		      m[1] * m[7] * m[10] + 
		      m[5] * m[2] * m[11] - 
		      m[5] * m[3] * m[10] - 
		      m[9] * m[2] * m[7] + 
		      m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] - 
		     m[0] * m[7] * m[10] - 
		     m[4] * m[2] * m[11] + 
		     m[4] * m[3] * m[10] + 
		     m[8] * m[2] * m[7] - 
		     m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] + 
		       m[0] * m[7] * m[9] + 
		       m[4] * m[1] * m[11] - 
		       m[4] * m[3] * m[9] - 
		       m[8] * m[1] * m[7] + 
		       m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] - 
		      m[0] * m[6] * m[9] - 
		      m[4] * m[1] * m[10] + 
		      m[4] * m[2] * m[9] + 
		      m[8] * m[1] * m[6] - 
		      m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return true;
}
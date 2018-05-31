#ifndef GAMECOMMON_HEADER
#define GAMECOMMON_HEADER

#include <windows.h>
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <map>
#include <climits>
#include "Timer.h"
#include "Vector2.hpp"
#include "Vector2i.hpp"
#include "Vector3.h"
#include "Vector3i.hpp"
#include "AABB2.hpp"
#include "AABB3.hpp"
#include "glext.h"
#include "GLRender.hpp"
#include "Camera3D.hpp"
#include "RGBColor.hpp"

using namespace std;
// Put externs for these globals in some header that everyone (who cares about drawing) can see
extern PFNGLGENBUFFERSPROC		glGenBuffers;
extern PFNGLBINDBUFFERPROC		glBindBuffer;
extern PFNGLBUFFERDATAPROC		glBufferData;
extern PFNGLGENERATEMIPMAPPROC	glGenerateMipmap;
extern PFNGLDELETEBUFFERSPROC   glDeleteBuffers;

typedef struct _Vertex3D
{
	Vector3	m_position;
	RGBColor m_color;
	Vector2 m_texCoords;
} Vertex3D;

const unsigned int STRINGF_STACK_LOCAL_TEMP_LENGTH = 256;
//-----------------------------------------------------------------------------------------------
//Construct a temporary std::string using printf-style formatting.  For example:
//std::string studentInfoString = Stringf( "%s is a %d-year-old %s.", studentName, studentAge, studentGender );

inline const std::string Stringf( const char* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
//Same as the above version of Stringf(), but with an explicitly chosen maximum buffer size.

inline const std::string Stringf( const int maxLength, const char* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}



//-----------------------------------------------------------------------------------------------
//Print a string to the Visual Studio "Output" debug window, if we are running in the debugger.
//Uses printf-style formatting, for example:
//DebuggerPrintf( "%s is a %d-year-old %s.", studentName, studentAge, studentGender );

inline void DebuggerPrintf( const char* messageFormat, ... )
{
	const int MESSAGE_MAX_LENGTH = 2048;
	char messageLiteral[ MESSAGE_MAX_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, messageFormat );
	vsnprintf_s( messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, messageFormat, variableArgumentList );
	va_end( variableArgumentList );
	messageLiteral[ MESSAGE_MAX_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	//#if defined( PLATFORM_WINDOWS )
	//	if( IsDebuggerAvailable() )
	//	{
	OutputDebugStringA( messageLiteral ); // Print the string to the "Output" debug window in Visual Studio
	//	}
	//#endif

	std::cout << messageLiteral; // Also write it out to standard out, if anyone is listening
}

#endif
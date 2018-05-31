#include "Matrix44.hpp"

Matrix44::Matrix44()
{
	float temp[16] = { 1.f, 0.f, 0.f, 0.f,  
					   0.f, 1.f, 0.f, 0.f,
		               0.f, 0.f, 1.f, 0.f,
		               0.f, 0.f, 0.f, 1.f }; 

	for(unsigned int i = 0; i < 16; i++)
		m_matrix[i] = temp[i];
}

Matrix44::Matrix44(const float matrix[16])
{
	for(unsigned int i = 0; i < 16; i++)
		m_matrix[i] = matrix[i];
}

Matrix44 Matrix44::CreateIdentity()
{
	Matrix44 temp;
	float iden[16] = { 1.f, 0.f, 0.f, 0.f,
					   0.f, 1.f, 0.f, 0.f,
					   0.f, 0.f, 1.f, 0.f,
					   0.f, 0.f, 0.f, 1.f };

	for(unsigned int i = 0; i < 16; i++)
	{
		temp.m_matrix[i] = iden[i];
	}

	return temp;
}

Matrix44 Matrix44::CreateTranslationMatrix(const Vector3& translation)
{
	Matrix44 translationMatrix;
	translationMatrix.m_matrix[12] = translation.x;
	translationMatrix.m_matrix[13] = translation.y;
	translationMatrix.m_matrix[14] = translation.z;
	return translationMatrix;
}

Matrix44 Matrix44::CreateUniformScaleMatrix(float scale)
{
	Matrix44 scaleMatrix;
	scaleMatrix.m_matrix[0] = scale;
	scaleMatrix.m_matrix[5] = scale;
	scaleMatrix.m_matrix[10] = scale;
	return scaleMatrix;
}

Matrix44 Matrix44::CreateRotationAboutAxisDegrees(const Vector3& rotationAxis, float rotationDegree )
{
	Vector3 normalizedAxis = rotationAxis;
	normalizedAxis = normalizedAxis.Normalize();
	Vector3& u = normalizedAxis;

	Matrix44 rotationMatrix;

	float cosAngle = cos(MathUtilities::DegToRad(rotationDegree));
	float sinAngle = sin(MathUtilities::DegToRad(rotationDegree));
	float ic = 1.f - cosAngle;

	rotationMatrix.m_matrix[0] = cosAngle + u.x * u.x * ic;
	rotationMatrix.m_matrix[4] = u.x * u.y * ic - u.z * sinAngle;
	rotationMatrix.m_matrix[8] = u.x * u.z * ic + u.y * sinAngle;

	rotationMatrix.m_matrix[1] = u.y * u.x * ic + u.z * sinAngle;
	rotationMatrix.m_matrix[5] = cosAngle + u.y * u.y * ic;
	rotationMatrix.m_matrix[9] = u.y * u.z * ic - u.x * sinAngle;

	rotationMatrix.m_matrix[2] = u.z * u.x * ic - u.y * sinAngle;
	rotationMatrix.m_matrix[6] = u.z * u.y * ic + u.x * sinAngle;
	rotationMatrix.m_matrix[10] = cosAngle + u.z * u.z * ic;

	return rotationMatrix;
}

Matrix44 Matrix44::operator+(const Matrix44& other)
{
	Matrix44 temp;

	for(unsigned int i = 0; i < 16; i++)
		temp.m_matrix[i] = m_matrix[i] + other.m_matrix[i];

	return temp;
}

Matrix44 Matrix44::operator+=(const Matrix44& other)
{
	Matrix44 temp;
	for(unsigned int i = 0; i < 16; i++)
	{
		temp.m_matrix[i] = m_matrix[i];
	}

	for(unsigned int i = 0; i < 16; i++)
	{
		temp.m_matrix[i] += other.m_matrix[i];
	}

	return temp;
}

Matrix44 Matrix44::operator-(const Matrix44& other)
{
	Matrix44 temp;

	for(unsigned int i = 0; i < 16; i++)
		temp.m_matrix[i] = m_matrix[i] - other.m_matrix[i];

	return temp;
}

Matrix44 Matrix44::operator-=(const Matrix44& other)
{
	Matrix44 temp;
	for(unsigned int i = 0; i < 16; i++)
	{
		temp.m_matrix[i] = m_matrix[i];
	}

	for(unsigned int i = 0; i < 16; i++)
	{
		temp.m_matrix[i] -= other.m_matrix[i];
	}

	return temp;
}

void Matrix44::operator=(const Matrix44& other)
{
	for(unsigned int i = 0; i < 16; i++)
	{
		m_matrix[i] = other.m_matrix[i];
	}
}

bool Matrix44::operator==(const Matrix44& other)
{
	for(unsigned int i = 0; i < 16; i++)
	{
		if(m_matrix[i] != other.m_matrix[i])
			return false;
	}
	return true;
}

Vector3 Matrix44::TransformPoint(const Vector3& vector )
{
	Vector3 temp = vector;
	float x = temp.x;
	float y = temp.y;
	float z = temp.z;

	temp.x = m_matrix[0] * x + m_matrix[4] * y + m_matrix[8] * z + m_matrix[12];
	temp.y = m_matrix[1] * x + m_matrix[5] * y + m_matrix[9] * z + m_matrix[13];
	temp.z = m_matrix[2] * x + m_matrix[6] * y + m_matrix[10] * z + m_matrix[14];

	return temp;
}

Vector3 Matrix44::TransformDirection(const Vector3& vector)
{
	Vector3 temp = vector;
	float x = temp.x;
	float y = temp.y;
	float z = temp.z;

	temp.x = m_matrix[0] * x + m_matrix[4] * y + m_matrix[8] * z; 
	temp.y = m_matrix[1] * x + m_matrix[5] * y + m_matrix[9] * z;
	temp.z = m_matrix[2] * x + m_matrix[6] * y + m_matrix[10] * z;

	return temp;
}

Matrix44 Matrix44::TransformMatrix(const Matrix44& matrix)
{
	Matrix44 result;
	float lhs[16];
	float rhs[16];
	for(unsigned int i = 0; i < 16; i++)
	{
		rhs[i] = matrix.m_matrix[i];
		lhs[i] = m_matrix[i];
	}

// 	result.m_matrix[0] = lhs[0] * rhs[0] + lhs[1] * rhs[4] + lhs[2] * rhs[8] + lhs[3] * rhs[12];
// 	result.m_matrix[1] = lhs[0] * rhs[1] + lhs[1] * rhs[5] + lhs[2] * rhs[9] + lhs[3] * rhs[13];
// 	result.m_matrix[2] = lhs[0] * rhs[2] + lhs[1] * rhs[6] + lhs[2] * rhs[10] + lhs[3] * rhs[14];
// 	result.m_matrix[3] = lhs[0] * rhs[3] + lhs[1] * rhs[7] + lhs[2] * rhs[11] + lhs[3] * rhs[15];
// 
// 	result.m_matrix[4] = lhs[4] * rhs[0] + lhs[5] * rhs[4] + lhs[6] * rhs[8] + lhs[7] * rhs[12];
// 	result.m_matrix[5] = lhs[4] * rhs[1] + lhs[5] * rhs[5] + lhs[6] * rhs[9] + lhs[7] * rhs[13];
// 	result.m_matrix[6] = lhs[4] * rhs[2] + lhs[5] * rhs[6] + lhs[6] * rhs[10] + lhs[7] * rhs[14];
// 	result.m_matrix[7] = lhs[4] * rhs[3] + lhs[5] * rhs[7] + lhs[6] * rhs[11] + lhs[7] * rhs[15];
// 
// 	result.m_matrix[8] = lhs[8] * rhs[0] + lhs[9] * rhs[4] + lhs[10] * rhs[8] + lhs[11] * rhs[12];
// 	result.m_matrix[9] = lhs[8] * rhs[1] + lhs[9] * rhs[5] + lhs[10] * rhs[9] + lhs[11] * rhs[13];
// 	result.m_matrix[10] = lhs[8] * rhs[2] + lhs[9] * rhs[6] + lhs[10] * rhs[10] + lhs[11] * rhs[14];
// 	result.m_matrix[11] = lhs[8] * rhs[3] + lhs[9] * rhs[7] + lhs[10] * rhs[11] + lhs[11] * rhs[15];
// 
// 	result.m_matrix[12] = lhs[12] * rhs[0] + lhs[13] * rhs[4] + lhs[14] * rhs[8] + lhs[15] * rhs[12];
// 	result.m_matrix[13] = lhs[12] * rhs[1] + lhs[13] * rhs[5] + lhs[14] * rhs[9] + lhs[15] * rhs[13];
// 	result.m_matrix[14] = lhs[12] * rhs[2] + lhs[13] * rhs[6] + lhs[14] * rhs[10] + lhs[15] * rhs[14];
// 	result.m_matrix[15] = lhs[12] * rhs[3] + lhs[13] * rhs[7] + lhs[14] * rhs[11] + lhs[15] * rhs[15];

 	result.m_matrix[0] = lhs[0] * rhs[0] + lhs[4] * rhs[1] + lhs[8] * rhs[2] + lhs[12] * rhs[3];
	result.m_matrix[1] = lhs[1] * rhs[0] + lhs[5] * rhs[1] + lhs[9] * rhs[2] + lhs[13] * rhs[3];
	result.m_matrix[2] = lhs[2] * rhs[0] + lhs[6] * rhs[1] + lhs[10] * rhs[2] + lhs[14] * rhs[3];
	result.m_matrix[3] = lhs[3] * rhs[0] + lhs[7] * rhs[1] + lhs[11] * rhs[2] + lhs[15] * rhs[3];

	result.m_matrix[4] = lhs[0] * rhs[4] + lhs[4] * rhs[5] + lhs[8] * rhs[6] + lhs[12] * rhs[7];
	result.m_matrix[5] = lhs[1] * rhs[4] + lhs[5] * rhs[5] + lhs[9] * rhs[6] + lhs[13] * rhs[7];
	result.m_matrix[6] = lhs[2] * rhs[4] + lhs[6] * rhs[5] + lhs[10] * rhs[6] + lhs[14] * rhs[7];
	result.m_matrix[7] = lhs[3] * rhs[4] + lhs[7] * rhs[5] + lhs[11] * rhs[6] + lhs[15] * rhs[7];

	result.m_matrix[8] = lhs[0] * rhs[8] + lhs[4] * rhs[9] + lhs[8] * rhs[10] + lhs[12] * rhs[11];
	result.m_matrix[9] = lhs[1] * rhs[8] + lhs[5] * rhs[9] + lhs[9] * rhs[10] + lhs[13] * rhs[11];
	result.m_matrix[10] = lhs[2] * rhs[8] + lhs[6] * rhs[9] + lhs[10] * rhs[10] + lhs[14] * rhs[11];
	result.m_matrix[11] = lhs[3] * rhs[8] + lhs[7] * rhs[9] + lhs[11] * rhs[10] + lhs[15] * rhs[11];

	result.m_matrix[12] = lhs[0] * rhs[12] + lhs[4] * rhs[13] + lhs[8] * rhs[14] + lhs[12] * rhs[15];
	result.m_matrix[13] = lhs[1] * rhs[12] + lhs[5] * rhs[13] + lhs[9] * rhs[14] + lhs[13] * rhs[15];
	result.m_matrix[14] = lhs[2] * rhs[12] + lhs[6] * rhs[13] + lhs[10] * rhs[14] + lhs[14] * rhs[15];
	result.m_matrix[15] = lhs[3] * rhs[12] + lhs[7] * rhs[13] + lhs[11] * rhs[14] + lhs[15] * rhs[15];


	return result;
}

Matrix44 Matrix44::CreatePerspectiveMatrix(float fovY,float aspect,float zNear,float zFar)
{
	float f = 1.f / ( tan( MathUtilities::DegToRad( fovY * 0.5f ) ) );
	Matrix44 perspectiveMatrix;

	perspectiveMatrix.m_matrix[0] = f / aspect;
	perspectiveMatrix.m_matrix[5] = f;
	perspectiveMatrix.m_matrix[10] = ( zFar + zNear ) / ( zNear - zFar );
	perspectiveMatrix.m_matrix[11] = -1.f;
	perspectiveMatrix.m_matrix[14] = ( 2 * zFar * zNear ) / ( zNear - zFar );
	perspectiveMatrix.m_matrix[15] = 0.f;

	return perspectiveMatrix;
}

Matrix44 Matrix44::CreateOrthoMatrix(float left, float right, float bottom, float top, float near, float far)
{
	Matrix44 orthoMatrix;

	float tx = ( right  + left ) / ( right - left );
	float ty = ( top + bottom ) / ( top - bottom );
	float tz = ( far + near ) / ( far - near );

	orthoMatrix.m_matrix[0] = 2.f / ( right - left );
	orthoMatrix.m_matrix[5] = 2.f / ( top - bottom );
	orthoMatrix.m_matrix[10] = -2.f / ( far - near );
	orthoMatrix.m_matrix[12] = - ( right + left ) / ( right - left );//tx;
	orthoMatrix.m_matrix[13] = - ( top + bottom ) / ( top - bottom );//ty;
	orthoMatrix.m_matrix[14] = - ( far + near ) / ( far - near );//tz;
	orthoMatrix.m_matrix[15] = 1.f;

	return orthoMatrix;
}
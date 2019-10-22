#include "Shapes.h"

#define _USE_MATH_DEFINES
#include <math.h>


const std::vector<float>& Shape::GetVertexPositions() const
{
	return m_VertexPositions;
}

const std::vector<unsigned int>& Shape::GetIndices() const
{
	return m_Indices;
}

Line::Line(float startX, float startY, float endX, float endY)
{
	m_VertexPositions.push_back(startX);
	m_VertexPositions.push_back(startY);
	m_VertexPositions.push_back(startX + (float)0.01);	// change so this isn't a magic number
	m_VertexPositions.push_back(startY + (float)0.01);
	m_VertexPositions.push_back(endX + (float)0.01);
	m_VertexPositions.push_back(endY + (float)0.01);
	m_VertexPositions.push_back(endX);
	m_VertexPositions.push_back(endY);

	unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };
	m_Indices.insert(m_Indices.end(), indices, indices + 6);
}

// Constructer overload to create line using polar with a start point
// Returns the computed end point in endXPtr and endYPtr
Line::Line(float startX, float startY, float distFromStart, float angle, bool inRadians, float *endXPtr, float *endYPtr)
{
	// Compute the end point
	if (!inRadians)
		angle = angle * M_PI / 180;
	*endXPtr = startX + distFromStart * cos(angle);
	*endYPtr = startY + distFromStart * sin(angle);

	m_VertexPositions.push_back(startX);
	m_VertexPositions.push_back(startY);
	m_VertexPositions.push_back(startX + (float)0.01);	// change so this isn't a magic number
	m_VertexPositions.push_back(startY + (float)0.01);
	m_VertexPositions.push_back(*endXPtr + (float)0.01);
	m_VertexPositions.push_back(*endYPtr + (float)0.01);
	m_VertexPositions.push_back(*endXPtr);
	m_VertexPositions.push_back(*endYPtr);

	unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };
	m_Indices.insert(m_Indices.end(), indices, indices + 6);
}

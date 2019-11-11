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

Polygon::Polygon(std::vector<std::pair<float, float>> vertices, std::array <float, colourVectorLength> RGBA)	// Maybe should use a struct instead of a pair
{
	if (vertices.size() < 3) printf("Error, cannot initialize polygon with less than 3 vertices\n");
	else if (vertices.size() > 10) printf("Error, max vertices limit hit for polygon\n");

	for (int i = 0; i < colourVectorLength; i++) {
		m_Colour[i] = RGBA[i];
	}

	for (unsigned int i = 0; i < vertices.size(); i++) {
		m_VertexPositions.push_back(vertices[i].first);
		m_VertexPositions.push_back(vertices[i].second);
	}

	switch (vertices.size()) {
		case 3:
		{
			unsigned int triangleIndices[3] = { 0, 1, 2 };
			m_Indices.insert(m_Indices.end(), triangleIndices, triangleIndices + 3);
			break;
		}
		case 4:
		{
			unsigned int rectIndices[6] = { 0, 1, 2, 2, 3, 0 };
			m_Indices.insert(m_Indices.end(), rectIndices, rectIndices + 6);
			break;
		}
	}
}

Polygon::Polygon(std::vector<std::pair<float, float>> vertices) : Polygon(vertices, DEFAULT_COLOUR)
{
}

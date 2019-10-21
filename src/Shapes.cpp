#include "Shapes.h"

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

Line::Line(float startX, float startY, float distFromStart, float angles, bool inRadians)
{

}

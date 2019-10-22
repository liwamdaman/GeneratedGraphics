#pragma once

#include <string>
#include <vector>

class Shape
{
protected:
	std::vector <float> m_VertexPositions;
	std::vector <unsigned int> m_Indices;
	std::string m_colour; // TODO: implement this - map colour strings to RGBA colour vectors and then use those in a shader

public:
	const std::vector<float>& GetVertexPositions() const;
	const std::vector<unsigned int>& GetIndices() const;
};

class Line : public Shape
{
public:
	Line(float startX, float startY, float endX, float endY);
	Line(float startX, float startY, float distFromStart, float angle, bool inRadians, float* endXPtr, float* endYPtr);
};
#pragma once

#include <string>
#include <vector>
#include <array>

#include "ColourValues.h"

class Shape
{
public:
	/* Constants */
	static const unsigned int colourVectorLength = 4;

protected:
	std::vector <float> m_VertexPositions;
	std::vector <unsigned int> m_Indices;

public:
	float m_Colour[colourVectorLength]; // RGBA value	

	const std::vector<float>& GetVertexPositions() const;
	const std::vector<unsigned int>& GetIndices() const;
};

class Line : public Shape
{
public:
	Line(float startX, float startY, float endX, float endY);
	Line(float startX, float startY, float distFromStart, float angle, bool inRadians, float* endXPtr, float* endYPtr);
};

class Polygon : public Shape
{
public:
	Polygon(std::vector<std::pair<float, float>> vertices, std::array <float, colourVectorLength> RGBA);
	Polygon(std::vector<std::pair<float, float>> vertices);
};
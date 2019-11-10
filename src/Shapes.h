#pragma once

#include <string>
#include <vector>
#include <array>

class Shape
{
protected:
	/* Constants */
	static const int colourVectorLength = 4;
	const std::array<float, colourVectorLength> defaultColour = { 1.0, 0.0, 0.0, 1.0 };	// Red

	std::vector <float> m_VertexPositions;
	std::vector <unsigned int> m_Indices;
	float m_Colour[colourVectorLength]; // RGBA value	

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

class Polygon : public Shape
{
public:
	Polygon(std::vector<std::pair<float, float>> vertices, std::array <float, 4> RGBA);
	Polygon(std::vector<std::pair<float, float>> vertices);
};
#include <iostream>

#include "Graphic.h"

Graphic::Graphic()
{
	m_VbPtr = NULL;
	m_VaPtr = NULL;
	m_IbPtr = NULL;
	m_ShaderPtr = NULL;
}

const VertexArray& Graphic::GetVertexArray() const
{
	return *(this->m_VaPtr);
}

const IndexBuffer& Graphic::GetIndexBuffer() const
{
	return *(this->m_IbPtr);
}

Shader Graphic::GetShader() const
{
	return *(this->m_ShaderPtr);
}

int Graphic::AddShape(std::vector<float> * vertexDataBufferPtr, std::vector<unsigned int> *indexBufferPtr, Shape shape, bool addColour) const
{
	// Implement some max size possibly? so that we can't infinitely add shapes and increase the size of positionBuffer

	/* TODO: instead of simply appending this shape to the buffers, see if previous positions have already been used to 
	try to minimize number of positions and indices used */

	unsigned int offset = 0;
	if (addColour) {
		// Assume that all the data in buffer currently is two floats for position and four floats for colour per vertex
		offset = vertexDataBufferPtr->size() / 6;

		for (unsigned int i = 0; i < shape.GetVertexPositions().size(); i = i + 2) {
			vertexDataBufferPtr->push_back(shape.GetVertexPositions()[i]);
			vertexDataBufferPtr->push_back(shape.GetVertexPositions()[i+1]);
			for (unsigned int j = 0; j < shape.colourVectorLength; j++) {
				vertexDataBufferPtr->push_back(shape.m_Colour[j]);
			}
		}
	}
	else {
		offset = vertexDataBufferPtr->size() / 2;

		vertexDataBufferPtr->insert(vertexDataBufferPtr->end(), shape.GetVertexPositions().begin(), shape.GetVertexPositions().end());
	}

	for (unsigned int i = 0; i < shape.GetIndices().size(); i++) {
		indexBufferPtr->push_back(shape.GetIndices()[i] + offset);
	}


	return 0;	// TODO: add exception handling and use other return values
}

float Graphic::MapPixelCoordToScreenCoord(unsigned int pixelCoord, bool isXCoord) const
{
	/* From observation, OpenGL screen goes from -1 to 1 in both x and y directions */
	if (isXCoord)
		return (float)pixelCoord * 2 / DEFAULT_NATIVE_RESOLUTION_WIDTH - 1;
	else
		return (float)pixelCoord * 2 / DEFAULT_NATIVE_RESOLUTION_HEIGHT - 1;
}

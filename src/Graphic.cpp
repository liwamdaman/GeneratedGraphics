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

int Graphic::AddShape(std::vector<float> *positionBufferPtr, std::vector<unsigned int> *indexBufferPtr, Shape shape)
{
	// Implement some max size possibly? so that we can't infinitely add shapes and increase the size of positionBuffer

	/* TODO: instead of simply appending this shape to the buffers, see if previous positions have already been used to 
	try to minimize number of positions and indices used */

	unsigned int offset = positionBufferPtr->size();

	positionBufferPtr->insert(positionBufferPtr->end(), shape.GetVertexPositions().begin(), shape.GetVertexPositions().end());
	
	for (int i = 0; i < shape.GetIndices().size(); i++) {
		indexBufferPtr->push_back(shape.GetIndices()[i] + offset);
	}

	return 0;
}

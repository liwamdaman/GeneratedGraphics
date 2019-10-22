#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Shapes.h"


class Graphic
{
protected:
	VertexBuffer *m_VbPtr;
	VertexArray* m_VaPtr;
	IndexBuffer* m_IbPtr;
	Shader* m_ShaderPtr; // Want to be able to use individual shaders for each graphic type, all stored on different paths perhaps
public:
	Graphic();

	const VertexArray& GetVertexArray() const;
	const IndexBuffer& GetIndexBuffer() const;
	Shader GetShader() const; // Doesn't return const reference so that application code can directly set uniform

protected:
	int AddShape(std::vector<float> *positionBuffer, std::vector<unsigned int> * indexBuffer, Shape shape); // might need to be virtual

};
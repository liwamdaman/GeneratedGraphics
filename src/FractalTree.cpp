#include "FractalTree.h"
#include "VertexBufferLayout.h"

FractalTree::FractalTree()	// Default to depth of ten and 2 branches per parent
{
	std::vector<float> positionBuffer;
	std::vector<unsigned int> indexBuffer;
	Line line = Line(0.0f, 0.0f, 0.0f, 0.5f);
	AddShape(&positionBuffer, &indexBuffer, line);

	m_VbPtr = new VertexBuffer(&positionBuffer[0], positionBuffer.size() * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	m_VaPtr = new VertexArray();
	m_VaPtr->AddBuffer(*m_VbPtr, layout);
	m_IbPtr = new IndexBuffer(&indexBuffer[0], indexBuffer.size());
	m_ShaderPtr = new Shader("res/shaders/Basic.shader");
	m_ShaderPtr->Bind();
	m_ShaderPtr->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

	//float positions[] = {
	//	/*-0.5,	-0.5,
	//	 0.5,	-0.5,
	//	 0.5,	 0.5,
	//	-0.5,	 0.5,*/
	//	0.0,	0.0,
	//	0.01,	0.01,
	//	0.51,	0.01,
	//	0.5,	0.0
	//};

	//unsigned int indices[] = {
	//	0, 1, 2,
	//	2, 3, 0
	//};

	//m_Va = new VertexArray();
	//m_Vb = new VertexBuffer(positions, 4 * 2 * sizeof(float));

	//VertexBufferLayout layout;
	//layout.Push<float>(2);
	//m_Va->AddBuffer(*m_Vb, layout);

	//m_Ib = new IndexBuffer(indices, 6);

	//m_Shader = new Shader("res/shaders/Basic.shader");
	//m_Shader->Bind();
	//m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	//std::cout << m_Shader->m_RendererID << std::endl;
}

FractalTree::FractalTree(unsigned int depth, unsigned int branchNum)
{
}

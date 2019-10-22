#include "FractalTree.h"
#include "VertexBufferLayout.h"

FractalTree::FractalTree()	// Default to depth of ten and 2 branches per parent
{
	std::vector<float> positionBuffer;
	std::vector<unsigned int> indexBuffer;

	/*Line line = Line(0.0f, 0.0f, 0.0f, 0.5f);
	AddShape(&positionBuffer, &indexBuffer, line);
	Line line2 = Line(0.0f, 0.5f, 0.1f, 0.9f);
	AddShape(&positionBuffer, &indexBuffer, line2);
	Line line3 = Line(0.0f, 0.5f, -0.1f, 0.9f);
	AddShape(&positionBuffer, &indexBuffer, line3);
	float* endXPtr = new float;
	float* endYPtr = new float;
	Line line4 = Line(0.0f, 0.0f, 0.5f, 0.4f, true, endXPtr, endYPtr);
	AddShape(&positionBuffer, &indexBuffer, line4);
	Line line5 = Line(-0.3f, 0.2f, 0.7f, 300.0f, false, endXPtr, endYPtr);
	AddShape(&positionBuffer, &indexBuffer, line5);*/

	// Create tree
	float* endXPtr = new float;
	float* endYPtr = new float;
	CreateBranches(DEFAULT_DEPTH, 0, &positionBuffer, &indexBuffer, 0.0, -0.5, MAIN_STEM_LENGTH, 90.0, endXPtr, endYPtr);

	// Create VertexBuffer, VertexArray, IndexBuffer, Shader
	m_VbPtr = new VertexBuffer(&positionBuffer[0], positionBuffer.size() * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	m_VaPtr = new VertexArray();
	m_VaPtr->AddBuffer(*m_VbPtr, layout);
	m_IbPtr = new IndexBuffer(&indexBuffer[0], indexBuffer.size());
	m_ShaderPtr = new Shader("res/shaders/Basic.shader");
	m_ShaderPtr->Bind();
	m_ShaderPtr->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

	/* Test code */
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

void FractalTree::CreateBranches(unsigned int maxDepth, unsigned int currDepth, 
									std::vector<float>* positionBuffer, std::vector<unsigned int>* indexBuffer, 
									float startX, float startY, float distFromStart, float angle,
									float *endXPtr, float *endYPtr)
{
	if (currDepth >= maxDepth)
		return;
	currDepth++;

	Line line = Line(startX, startY, distFromStart, angle, false, endXPtr, endYPtr);	// TODO: maybe implement support for using radians?
	AddShape(positionBuffer, indexBuffer, line);

	float nextStartX = *endXPtr;
	float nextStartY = *endYPtr;
	CreateBranches(maxDepth, currDepth, positionBuffer, indexBuffer, nextStartX, nextStartY, 
		distFromStart * STEM_LENGTH_SHRINK_RATE, angle + BRANCH_SPREAD_ANGLE, endXPtr, endYPtr);
	CreateBranches(maxDepth, currDepth, positionBuffer, indexBuffer, nextStartX, nextStartY, 
		distFromStart * STEM_LENGTH_SHRINK_RATE, angle - BRANCH_SPREAD_ANGLE, endXPtr, endYPtr);
}

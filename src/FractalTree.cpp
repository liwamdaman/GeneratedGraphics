#include "FractalTree.h"
#include "VertexBufferLayout.h"

FractalTree::FractalTree(unsigned int depth, unsigned int branchesPerNode)
{
	unsigned int numLines = pow(branchesPerNode, (depth - INITIAL_DEPTH_LEVEL - 1)) + 1;
	if (numLines > MAX_LINES) {
		// Reduce depth to be under threshold. Sub in MAX_LINES for numLines, solve for depth, and truncate to int
		depth = (int)(log(MAX_LINES - 1) / log(branchesPerNode) + INITIAL_DEPTH_LEVEL + 1);
	}

	std::vector<float> positionBuffer;
	std::vector<unsigned int> indexBuffer;

	// Create tree
	float* endXPtr = new float;
	float* endYPtr = new float;
	CreateBranches(INITIAL_DEPTH_LEVEL, depth, branchesPerNode, &positionBuffer, &indexBuffer, 
					BASE_X_COORD, BASE_Y_COORD, MAIN_STEM_LENGTH, 90.0, endXPtr, endYPtr);

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
}

FractalTree::FractalTree() : FractalTree(DEFAULT_DEPTH, DEFAULT_BRANCHES_PER_NODE)	// Default to depth of ten and 2 branches per parent
{
}

void FractalTree::CreateBranches(unsigned int currDepth, unsigned int maxDepth, unsigned int branchesPerNode,
									std::vector<float>* vertexDataBufferPtr, std::vector<unsigned int>* indexBuffer,
									float startX, float startY, float distFromStart, float angle,
									float *endXPtr, float *endYPtr)
{
	if (currDepth >= maxDepth)
		return;
	currDepth++;

	Line line = Line(startX, startY, distFromStart, angle, false, endXPtr, endYPtr);	// TODO: maybe implement support for using radians?
	AddShape(vertexDataBufferPtr, indexBuffer, line, false);

	float nextStartX = *endXPtr;
	float nextStartY = *endYPtr;

	for (unsigned int i = 0; i < branchesPerNode; i++) {
		CreateBranches(currDepth, maxDepth, branchesPerNode, vertexDataBufferPtr, indexBuffer, nextStartX, nextStartY,
			distFromStart * STEM_LENGTH_SHRINK_RATE, angle - TOTAL_SPREAD_ANGLE/2 + i*TOTAL_SPREAD_ANGLE/(branchesPerNode-1),
			endXPtr, endYPtr);
	}

	/* Test Code to hard code lines instead of creating tree*/
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
}

void FractalTree::SetupGrid(unsigned int height, unsigned int width)
{
	printf("please don't call this lol, not supported for FractalTree type\n");
	// Perhaps SetUpGrid shouldn't be a base virtual function if not all derived class implement it
}

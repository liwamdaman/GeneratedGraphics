#include "Maze.h"
#include <iostream>


Maze::Maze(unsigned int width, unsigned int height, unsigned int startX, unsigned int startY)
{
	if (width > MAX_WIDTH) width = MAX_WIDTH;
	if (height > MAX_HEIGHT) height = MAX_HEIGHT;

	// Setup grid
	m_Width = width;
	m_Height = height;
	m_Nodes = NULL;
	SetupGrid(width, height);

	// Setup start point/node
	Node* startNode = m_Nodes + (startY * height + startX) * sizeof(Node);

	m_Layout.Push<float>(2);	// x and y position
	m_Layout.Push<float>(4);	// RGBA colour value

}

Maze::Maze() : Maze(32, 18, 0, 0)
{
}

void Maze::SetupGrid(unsigned int width, unsigned int height)
{
	m_Nodes = (Node*)malloc(width * height * sizeof(Node)); // Ignore this red underline, I think its a VS bug:https://github.com/Microsoft/vscode-cpptools/issues/3212
	if (m_Nodes == NULL) printf("Uh oh");

	Node* n;
	for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; j < width; j++) {
			n = m_Nodes + i * width + j;	// Pointer arithmetic: iterate row by row, increasing in height 
			n->xIndex = j;
			n->yIndex = i;
			if (i % 2 == 0)
				n->type = 0;
			else
				n->type = 1;
			n->dirs = 15;
		}
	}
}

void Maze::Iterate()
{
	float nodeWidth = (float)DEFAULT_NATIVE_RESOLUTION_WIDTH / (float)m_Width;	// In pixels
	float nodeHeight = (float)DEFAULT_NATIVE_RESOLUTION_HEIGHT / (float)m_Height;	// In pixels

	std::vector<float> vertexDataBuffer;
	std::vector<unsigned int> indexBuffer;

	Node* n;
	for (unsigned int i = 0; i < m_Height; i++) {
		for (unsigned int j = 0; j < m_Width; j++) {
			n = m_Nodes + i * m_Width + j;	// Pointer arithmetic: iterate row by row, increasing in height 
			
			/* build rectangle at this node and add to positionBuffer */
			std::vector<std::pair<float, float>> rectVertices;
			std::pair<float, float> botLeft(MapPixelCoordToScreenCoord(j * nodeWidth, true), MapPixelCoordToScreenCoord(i * nodeHeight, false));
			std::pair<float, float> botRight(MapPixelCoordToScreenCoord(j * nodeWidth + nodeWidth, true), MapPixelCoordToScreenCoord(i * nodeHeight, false));
			std::pair<float, float> topRight(MapPixelCoordToScreenCoord(j * nodeWidth + nodeWidth, true), MapPixelCoordToScreenCoord(i * nodeHeight + nodeHeight, false));
			std::pair<float, float> topLeft(MapPixelCoordToScreenCoord(j * nodeWidth, true), MapPixelCoordToScreenCoord(i * nodeHeight + nodeHeight, false));
			rectVertices.push_back(botLeft);
			rectVertices.push_back(botRight);
			rectVertices.push_back(topRight);
			rectVertices.push_back(topLeft);

			std::array <float, 4> RGBA;
			if (n->type == 0)
				RGBA = { 0.0,0.0,0.0,0.0 };
			else
				RGBA = { 1.0,0.0,0.0,0.0 };
			Polygon rectangle(rectVertices, RGBA);
			AddShape(&vertexDataBuffer, &indexBuffer, rectangle, true);
		}
	}

	/* Reassign buffers */
	m_VbPtr = new VertexBuffer(&vertexDataBuffer[0], vertexDataBuffer.size() * sizeof(float));
	m_VaPtr = new VertexArray();
	m_VaPtr->AddBuffer(*m_VbPtr, m_Layout);
	m_IbPtr = new IndexBuffer(&indexBuffer[0], indexBuffer.size());
	m_ShaderPtr = new Shader("res/shaders/Maze.shader");
	m_ShaderPtr->Bind();
}

Maze::Node* Maze::FindNextNode(Node* currNode)
{
	return nullptr;
}

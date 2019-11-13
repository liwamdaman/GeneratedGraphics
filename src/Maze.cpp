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
	m_StartNode = m_Nodes + startY * width + startX;	// Pointer arithmetic, don't need to multiply by sizeof(Node)

	m_Layout.Push<float>(2);	// x and y position
	m_Layout.Push<float>(4);	// RGBA colour value
}

Maze::Maze() : Maze(32, 18, 0, 0)
{
}

void Maze::SetupGrid(unsigned int width, unsigned int height)
{
	m_Nodes = (Node*)malloc(width * height * sizeof(Node)); // Ignore this red underline, I think its a VS bug:https://github.com/Microsoft/vscode-cpptools/issues/3212
	if (m_Nodes == NULL) printf("Uh oh\n");

	Node* n;
	for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; j < width; j++) {
			n = m_Nodes + i * width + j;	// Pointer arithmetic: iterate row by row, increasing in height 
			n->xIndex = j;
			n->yIndex = i;
			n->state = 0;
			n->dirs = 15;
		}
	}
}

// Iterate maze generation algorithm, and update for next draw
// Returns: 0 - ok, -1 - error, 1 - complete
int Maze::Iterate()
{
	float nodeWidth = (float)DEFAULT_NATIVE_RESOLUTION_WIDTH / (float)m_Width;	// In pixels
	float nodeHeight = (float)DEFAULT_NATIVE_RESOLUTION_HEIGHT / (float)m_Height;	// In pixels

	/* Determine next node to visit and mutate*/
	if (m_PathStack.empty()) {
		m_StartNode->state = 1;
		m_PathStack.push(m_StartNode);
	}
	//else {
	//	Node* nextNode = FindNextNode(m_PathStack.top());
	//	if (nextNode == NULL) {
	//		printf("ERROR: Failed to find next node\n");
	//		return -1;
	//	}
	//	if (nextNode == m_PathStack.top()) {
	//		// Cannot dig furthur, need to backtrack
	//		nextNode->state = 2;
	//		m_PathStack.pop();
	//	}
	//	else {
	//		nextNode->state = 1;
	//		m_PathStack.push(nextNode);
	//	}
	//}

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
			if (n->state == EMPTY_OR_WALL)
				RGBA = BLACK;
			else if (n->state == CROSSED_ONCE)
				RGBA = WHITE;
			else if (n->state == BACKTRACKED)
				RGBA = BLUE;
			else {
				printf("ERROR: Invalid node state\n");
				return -1;
			}
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

	return 0;
}

Maze::Node* Maze::FindNextNode(Node* currNode)
{
	if (currNode == NULL) return NULL;

	while (currNode->dirs) {

	}

}

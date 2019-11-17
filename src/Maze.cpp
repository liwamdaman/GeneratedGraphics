#include "Maze.h"
#include <iostream>


Maze::Maze(unsigned int width, unsigned int height, unsigned int startX, unsigned int startY, bool isWacky)
{
	if (width > MAX_WIDTH) width = MAX_WIDTH;
	if (height > MAX_HEIGHT) height = MAX_HEIGHT;

	if (startX >= width) startX = width - 1;
	if (startY >= height) startY = height - 1;

	// Setup grid
	m_Width = width;
	m_Height = height;
	m_Nodes = NULL;
	SetupGrid(width, height);

	// Setup start point/node
	m_StartNode = m_Nodes + startY * width + startX;	// Pointer arithmetic, don't need to multiply by sizeof(Node)

	m_Layout.Push<float>(2);	// x and y position
	m_Layout.Push<float>(Shape::colourVectorLength);	// RGBA colour value

	if (isWacky)
		m_IsWacky = true;
	else
		m_IsWacky = false;
}

Maze::Maze() : Maze(16, 9, 0, 0, false)
{
}

void Maze::SetupGrid(unsigned int width, unsigned int height)
{
	m_Nodes = (Node*)malloc(width * height * sizeof(Node)); // Ignore this red underline, I think its a VS bug:https://github.com/Microsoft/vscode-cpptools/issues/3212
	if (m_Nodes == NULL) printf("Uh oh\n");

	float nodeWidth = (float)DEFAULT_NATIVE_RESOLUTION_WIDTH / (float)m_Width;	// In pixels
	float nodeHeight = (float)DEFAULT_NATIVE_RESOLUTION_HEIGHT / (float)m_Height;	// In pixels

	Node* n;
	for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; j < width; j++) {
			n = m_Nodes + i * width + j;	// Pointer arithmetic: iterate row by row, increasing in height 
			n->xIndex = j;
			n->yIndex = i;
			n->state = 0;

			// Edge nodes should not have all directions available
			n->dirs = 0b1111;
			if (n->xIndex == 0) n->dirs &= ~0b0100;
			if (n->xIndex == width - 1) n->dirs &= ~0b01;
			if (n->yIndex == 0) n->dirs &= ~0b10;
			if (n->yIndex == height - 1) n->dirs &= ~0b1000;

			/* build rectangle at this node */
			std::vector<std::pair<float, float>> rectVertices;
			std::pair<float, float> botLeft(MapPixelCoordToScreenCoord(j * nodeWidth, true), MapPixelCoordToScreenCoord(i * nodeHeight, false));
			std::pair<float, float> botRight(MapPixelCoordToScreenCoord(j * nodeWidth + nodeWidth, true), MapPixelCoordToScreenCoord(i * nodeHeight, false));
			std::pair<float, float> topRight(MapPixelCoordToScreenCoord(j * nodeWidth + nodeWidth, true), MapPixelCoordToScreenCoord(i * nodeHeight + nodeHeight, false));
			std::pair<float, float> topLeft(MapPixelCoordToScreenCoord(j * nodeWidth, true), MapPixelCoordToScreenCoord(i * nodeHeight + nodeHeight, false));
			rectVertices.push_back(botLeft);
			rectVertices.push_back(botRight);
			rectVertices.push_back(topRight);
			rectVertices.push_back(topLeft);

			Polygon rectangle(rectVertices, BLACK);
			AddShape(&m_VertexDataBuffer, &m_IndexBuffer, rectangle, true);
		}
	}
}

// Iterate maze generation algorithm, and update for next draw
// Returns: 0 - ok, -1 - error, 1 - complete
int Maze::Iterate()
{
	static unsigned int maxDepth = 0;
	static Node *endNode = NULL;

	if (m_PathStack.empty()) {
		/* First iteration, colour the start node */
		m_StartNode->state = CROSSED_ONCE;
		ColourNode(&m_VertexDataBuffer, m_StartNode, WHITE);
		m_PathStack.push(m_StartNode);
	}
	else {
		/* Determine next node to visit and mutate*/
		Node* nextNode = FindNextNode(m_PathStack.top());
		if (nextNode == NULL) {
			printf("ERROR: Failed to find next node\n");
			return -1;
		}
		if (nextNode == m_StartNode) {
			/* Maze generation complete - colour the endNode */
			ColourNode(&m_VertexDataBuffer, endNode, ORANGE);

			/* Reassign buffers */
			delete m_VbPtr;
			delete m_VaPtr;
			delete m_IbPtr;
			delete m_ShaderPtr;
			m_VbPtr = new VertexBuffer(&m_VertexDataBuffer[0], m_VertexDataBuffer.size() * sizeof(float));
			m_VaPtr = new VertexArray();
			m_VaPtr->AddBuffer(*m_VbPtr, m_Layout);
			m_IbPtr = new IndexBuffer(&m_IndexBuffer[0], m_IndexBuffer.size());
			m_ShaderPtr = new Shader("res/shaders/Maze.shader");
			m_ShaderPtr->Bind();

			return 1;
		}
		if (nextNode == m_PathStack.top()) {
			/* Cannot dig furthur, need to backtrack - check if endNode should be updated */
			if (m_PathStack.size() > maxDepth) {
				maxDepth = m_PathStack.size();
				endNode = m_PathStack.top();
			}

			nextNode->state = BACKTRACKED;
			ColourNode(&m_VertexDataBuffer, nextNode, LIGHT_BLUE);
			m_PathStack.pop();
		}
		else {
			/* Continue digging further */
			nextNode->state = CROSSED_ONCE;
			ColourNode(&m_VertexDataBuffer, nextNode, WHITE);
			m_PathStack.push(nextNode);
		}
	}

	/* Reassign buffers */
	delete m_VbPtr;
	delete m_VaPtr;
	delete m_IbPtr;
	delete m_ShaderPtr;
	m_VbPtr = new VertexBuffer(&m_VertexDataBuffer[0], m_VertexDataBuffer.size() * sizeof(float));
	m_VaPtr = new VertexArray();
	m_VaPtr->AddBuffer(*m_VbPtr, m_Layout);
	m_IbPtr = new IndexBuffer(&m_IndexBuffer[0], m_IndexBuffer.size());
	m_ShaderPtr = new Shader("res/shaders/Maze.shader");
	m_ShaderPtr->Bind();

	return 0;
}

Maze::Node* Maze::FindNextNode(Node* currNode)
{
	if (currNode == NULL) return NULL;

	char dir;
	Node* nextNode = currNode;

	// While directions unexplored
	while (currNode->dirs) {
		// Randomly pick direction
		dir = 1 << (rand() % 4);

		// If it has already been explored - try again
		if (!(currNode->dirs & dir)) continue;

		currNode->dirs &= ~dir;

		Node* n;
		switch (dir) {

			case 1: 
				n = currNode + 1;
				break;
			case 2:
				n = currNode - m_Width;
				break;
			case 4:
				n = currNode - 1;
				break;
			case 8:
				n = currNode + m_Width;
				break;
			default: 
				printf("ERROR: invalid value for direction\n");
				n = currNode;
		}

		/* Check if this node is appropriate to go to next or not */
		if (n == NULL) {
			printf("ERROR: null ptr\n");
			continue;
		}
		if (n->state != EMPTY) continue;
		if (TouchingAnyOtherNodes(n, dir)) continue;
		nextNode = n;
		return nextNode;
	}

	// Have explored all possible paths with no success, we are at a dead end
	return currNode;
}

bool Maze::TouchingAnyOtherNodes(Node *currNode, char directionFromParent)
{
	// Note: the node is allowed to touch its parent node
	if (currNode->xIndex != m_Width - 1 && directionFromParent != 0b0100)
		if ((currNode + 1)->state != EMPTY)	// right
			return true;
	if (currNode->yIndex != 0 && directionFromParent != 0b1000)
		if ((currNode - m_Width)->state != EMPTY)	// down
			return true;
	if (currNode->xIndex != 0 && directionFromParent != 0b01)
		if ((currNode - 1)->state != EMPTY)	// left
			return true;
	if (currNode->yIndex != m_Height - 1 && directionFromParent != 0b10)
		if ((currNode + m_Width)->state != EMPTY)	// up
			return true;

	if (!m_IsWacky) {
		/* Apply more restrictions */
		if (currNode->xIndex != m_Width - 1 && currNode->yIndex != m_Height - 1 && directionFromParent != 0b0100 && directionFromParent != 0b0010)
			if((currNode + m_Width + 1)->state != EMPTY)	// top-right
				return true;
		if (currNode->xIndex != m_Width - 1 && currNode->yIndex != 0 && directionFromParent != 0b0100 && directionFromParent != 0b1000)
			if ((currNode - m_Width + 1)->state != EMPTY)	// bot-right
				return true;
		if (currNode->xIndex != 0 && currNode->yIndex != 0 && directionFromParent != 0b0001 && directionFromParent != 0b1000)
			if ((currNode - m_Width - 1)->state != EMPTY)	// bot-left
				return true;
		if (currNode->xIndex != 0 && currNode->yIndex != m_Height - 1 && directionFromParent != 0b0001 && directionFromParent != 0b0010)
			if ((currNode + m_Width - 1)->state != EMPTY)	// top-left
				return true;
	}

	return false;
}

void Maze::ColourNode(std::vector<float>* vertexDataBufferPtr, Node* n, std::array<float, Shape::colourVectorLength> RGBA)
{
	if (n == NULL) {
		printf("ERROR: null ptr\n");
		return;
	}
	
	// Each node has four vertices, and each vertex has consists of six floats in the vertex data buffer
	unsigned int index = (n->yIndex * m_Width + n->xIndex) * 4 * 6;	// index of data buffer where this node begins
	for (int vertex = 0; vertex < 4; vertex++) {
		index = index + 2;
		for (int colourValue = 0; colourValue < Shape::colourVectorLength; colourValue++) {
			(*vertexDataBufferPtr)[index] = RGBA[colourValue];
			index++;
		}
	}
}

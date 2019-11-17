#include "CyclicCA.h"

CyclicCA::CyclicCA(unsigned int width, unsigned int height, unsigned int range, unsigned int threshold, unsigned int numStates, bool usingMoore)
{
	if (width > MAX_WIDTH) width = MAX_WIDTH;
	if (height > MAX_HEIGHT) height = MAX_HEIGHT;

	if (numStates > 8) numStates = 8;	// Black and ROYGBIV

	// TODO: support using ranges larger than one and using Von Neumann neighborhood

	m_Width = width;
	m_Height = height;
	m_NeighbourhoodRange = range;
	m_Threshold = threshold;
	m_NumStates = numStates;
	m_UsingMoore = usingMoore;
	m_Nodes = NULL;
	SetupGrid(width, height);

	m_Layout.Push<float>(2);	// x and y position
	m_Layout.Push<float>(Shape::colourVectorLength);	// RGBA colour value
}

CyclicCA::CyclicCA() : CyclicCA(64, 36, 1, 3, 3, true)	// Default to rule 313 by David Griffeath
{
}

void CyclicCA::SetupGrid(unsigned int width, unsigned int height)
{
	m_Nodes = (Node*)malloc(width * height * sizeof(Node)); 
	if (m_Nodes == NULL) printf("Uh oh\n");

	Node *n;
	for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; j < width; j++) {
			n = m_Nodes + i * width + j;	// Pointer arithmetic: iterate row by row, increasing in height 
			n->xIndex = j;
			n->yIndex = i;
			n->state = rand() % m_NumStates;
			n->prevState = 0;
		}
	}
}

void CyclicCA::Iterate()
{
	float nodeWidth = (float)DEFAULT_NATIVE_RESOLUTION_WIDTH / (float)m_Width;	// In pixels
	float nodeHeight = (float)DEFAULT_NATIVE_RESOLUTION_HEIGHT / (float)m_Height;	// In pixels

	/* Determine next node to visit and mutate*/
	
	std::vector<float> vertexDataBuffer;
	std::vector<unsigned int> indexBuffer;

	Node *n;
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

			/* Pick colour - this is really ugly since we are using preprocessor for colour defines */
			std::array <float, 4> RGBA;
			switch (n->state) {
				case 0:
					RGBA = BLACK;
					break;
				case 1:
					RGBA = RED;
					break;
				case 2:
					RGBA = ORANGE;
					break;
				case 3:
					RGBA = YELLOW;
					break;
				case 4:
					RGBA = GREEN;
					break;
				case 5:
					RGBA = BLUE;
					break;
				case 6:
					RGBA = INDIGO;
					break;
				case 7:
					RGBA = VIOLET;
					break;
				default:
					printf("ERROR: Invalid node state\n");
					return;
			}
			Polygon rectangle(rectVertices, RGBA);
			AddShape(&vertexDataBuffer, &indexBuffer, rectangle, true);

			/* update the state of the node if appropriate */
			n->prevState = n->state;
			UpdateState(n);
		}
	}

	/* Reassign buffers */
	delete m_VbPtr;
	delete m_VaPtr;
	delete m_IbPtr;
	delete m_ShaderPtr;
	m_VbPtr = new VertexBuffer(&vertexDataBuffer[0], vertexDataBuffer.size() * sizeof(float));
	m_VaPtr = new VertexArray();
	m_VaPtr->AddBuffer(*m_VbPtr, m_Layout);
	m_IbPtr = new IndexBuffer(&indexBuffer[0], indexBuffer.size());
	m_ShaderPtr = new Shader("res/shaders/Colour.shader");
	m_ShaderPtr->Bind();

	// TODO: Optimize!! nested for loop does not scale well with larger grids.
	// Possible solutions: - multithread to use more than one core - use GPU for the processing

	return;
}

void CyclicCA::UpdateState(Node *currNode)
{
	if (currNode == NULL) {
		printf("ERROR: null ptr\n");
		return;
	}
	
	unsigned int count = 0;

	if (currNode->xIndex != m_Width - 1)
		if ((currNode + 1)->state == (currNode->state + 1)%m_NumStates)	// right
			count++;
	if (currNode->yIndex != 0)
		if ((currNode - m_Width)->prevState == (currNode->state + 1) % m_NumStates)	// down
			count++;
	if (currNode->xIndex != 0)
		if ((currNode - 1)->prevState == (currNode->state + 1) % m_NumStates)	// left
			count++;
	if (currNode->yIndex != m_Height - 1)
		if ((currNode + m_Width)->state == (currNode->state + 1) % m_NumStates)	// up
			count++;

	if (m_UsingMoore) {
		/* check corner neighbours */
		if (currNode->xIndex != m_Width - 1 && currNode->yIndex != m_Height - 1)
			if ((currNode + m_Width + 1)->state == (currNode->state + 1) % m_NumStates)	// top-right
				count++;
		if (currNode->xIndex != m_Width - 1 && currNode->yIndex != 0)
			if ((currNode - m_Width + 1)->prevState == (currNode->state + 1) % m_NumStates)	// bot-right
				count++;
		if (currNode->xIndex != 0 && currNode->yIndex != 0)
			if ((currNode - m_Width - 1)->prevState == (currNode->state + 1) % m_NumStates)	// bot-left
				count++;
		if (currNode->xIndex != 0 && currNode->yIndex != m_Height - 1)
			if ((currNode + m_Width - 1)->state == (currNode->state + 1) % m_NumStates)	// top-left
				count++;
	}

	if (count >= m_Threshold)
		currNode->state = (currNode->state + 1) % m_NumStates;

	return;
}


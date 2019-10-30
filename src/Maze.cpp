#include "Maze.h"

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

}

Maze::Maze() : Maze(32, 18, 0, 0)
{
}

void Maze::SetupGrid(unsigned int width, unsigned int height)
{
	m_Nodes = (Node*)malloc(width * height * sizeof(Node)); // Ignore this red underline, I think its a VS bug:https://github.com/Microsoft/vscode-cpptools/issues/3212
	if (m_Nodes == NULL) printf("Uh oh");

	Node* n;
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			n = m_Nodes + i * width + j;	// Iterate row by row, increasing in height
			n->x = i;
			n->y = j;
			n->type = 0;
			n->dirs = 15;
		}
	}
}

void Maze::Iterate()
{
	unsigned nodeWidth = DEFAULT_NATIVE_RESOLUTION_WIDTH / m_Width;	// In pixels
	unsigned nodeHeight = DEFAULT_NATIVE_RESOLUTION_HEIGHT / m_Height;	// In pixels


}

Maze::Node* Maze::FindNextNode(Node* currNode)
{
	return nullptr;
}

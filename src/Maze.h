#pragma once
#include "Graphic.h"
#include "VertexBufferLayout.h"
#include <string>
#include <stack>

/* Define a max resolution of 80 x 45*/
#define MAX_WIDTH 80
#define MAX_HEIGHT 45
#define NUM_VERTICES_PER_NODE 4

typedef enum {
	EMPTY,
	CROSSED_ONCE,
	BACKTRACKED
} NodeState;	// This is actually not necessary anymore, but will leave in for debugging purposes

class Maze : public Graphic
{
private:
	typedef struct
	{
		int xIndex, yIndex;
		//struct Node* parent;	//Pointer to parent node - Backtracking can be done using parent pointer or using a stack
		char state;	// This is actually not necessary anymore, but will leave in for debugging purposes
		char dirs;	// bitmap for directions that still haven't been explored - bits 0-3 represent availability for right, down, left, and up respectively
	} Node;

	Node* m_Nodes;	// Nodes array
	unsigned int m_Width, m_Height;
	std::stack<Node*> m_PathStack;	// Used for keeping track of what paths have been travelled - use pointers to save memory

	Node* m_StartNode;
	VertexBufferLayout m_Layout;
	bool m_IsWacky;
	
	std::vector<float> m_VertexDataBuffer;
	std::vector<unsigned int> m_IndexBuffer;

public:
	Maze(unsigned int width, unsigned int height, unsigned int startX, unsigned int startY, bool isWacky);	// turning on isWacky will generate the maze in a less visually organized manner
	Maze();

	int Iterate();	// Finds next node, and updates buffers
private:
	void SetupGrid(unsigned int width, unsigned int height);
	Node* FindNextNode(Node* currNode);
	bool TouchingAnyOtherNodes(Node* currNode, char directionApproachedFrom);
	void ColourNode(std::vector<float>* vertexDataBufferPtr, Node* n, std::array <float, Shape::colourVectorLength> RGBA);
};
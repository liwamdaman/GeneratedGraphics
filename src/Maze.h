#pragma once
#include "Graphic.h"
#include "VertexBufferLayout.h"
#include <string>
#include <stack>

/* Define a max resolution of 480 x 270*/
#define MAX_WIDTH 480
#define MAX_HEIGHT 270

typedef enum {

};

class Maze : public Graphic
{
private:
	typedef struct
	{
		int xIndex, yIndex;
		//struct Node* parent;	//Pointer to parent node - Backtracking can be done using parent pointer or using a stack
		char type;	// 0 - blank/wall, 1 - have gone over once, 2 - have backtracked over, map these to colours using an enum later
		char dirs;	// bitmap for directions that still haven't been explored - bits 0-3 represent availability for right, down, left, and up respectively
	} Node;

	Node* m_Nodes;	// Nodes array
	unsigned int m_Width, m_Height;
	std::stack<Node*> m_PathStack;	// Used for keeping track of what paths have been travelled - use pointers to save memory

	Node* m_StartNode;
	VertexBufferLayout m_Layout;
public:
	Maze(unsigned int width, unsigned int height, unsigned int startX, unsigned int startY);
	Maze();

	int Iterate();	// Finds next node, and updates buffers
private:
	void SetupGrid(unsigned int width, unsigned int height);
	Node* FindNextNode(Node* currNode);
};
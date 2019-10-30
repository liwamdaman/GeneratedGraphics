#pragma once
#include "Graphic.h"
#include <string>
#include <stack>

/* Define a max resolution of 480 x 270*/
#define MAX_WIDTH 480
#define MAX_HEIGHT 270

class Maze : public Graphic
{
private:
	typedef struct
	{
		int x, y;
		//struct Node* parent;	//Pointer to parent node - Backtracking can be done using parent pointer or using a stack
		char type;	// 0 - blank/wall, 1 - have gone over once, 2 - have backtracked over, map these to colours using an enum later
		char dirs;	// bitmap for directions that still haven't been explored - need to map directions to bits still
	} Node;

	Node* m_Nodes;	// Nodes array
	unsigned int m_Width, m_Height;
	std::stack<Node*> m_Path;	// Used for keeping track of what paths have been travelled - use pointers to save memory

	/* To store the actual position data that OpenGl needs from iteration to iteration 
		Its ugly that these need to be class members whereas they are just locally created in FractalTree constructor, consider refactor */
	std::vector<float> m_PositionBuffer;
	std::vector<unsigned int> m_IndexBuffer;
public:
	Maze(unsigned int width, unsigned int height, unsigned int startX, unsigned int startY);
	Maze();
private:
	void SetupGrid(unsigned int width, unsigned int height);
	void Iterate();	// Finds next node, and updates buffers
	Node* FindNextNode(Node* currNode);
};
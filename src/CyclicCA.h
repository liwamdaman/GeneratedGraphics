#pragma once
#include "Graphic.h"
#include "VertexBufferLayout.h"

/* Define a max resolution of 160 x 90*/
#define MAX_WIDTH 160
#define MAX_HEIGHT 90
#define NUM_VERTICES_PER_NODE 4

// Super similar to Maze, consider refactoring to inheriting from common base class such as GridGraphic
class CyclicCA : public Graphic
{
private:
	typedef struct
	{
		int xIndex, yIndex;
		char state;
		char prevState;
	} Node;

	Node* m_Nodes;	// Nodes array
	unsigned int m_Width, m_Height;

	unsigned int m_NeighbourhoodRange;
	unsigned int m_Threshold;
	unsigned int m_NumStates;
	bool m_UsingMoore;	// Alternative is using Von Neumann neighborhood

	VertexBufferLayout m_Layout;

	std::vector<float> m_VertexDataBuffer;
	std::vector<unsigned int> m_IndexBuffer;

public:
	CyclicCA(unsigned int width, unsigned int height, unsigned int range, unsigned int threshold, unsigned int numStates, bool usingMoore);
	CyclicCA();

	void Iterate();
private:
	void SetupGrid(unsigned int width, unsigned int height);
	void UpdateState(Node *currNode);
};

/* Useful references:
http://www.mirekw.com/ca/rullex_cycl.html
https://softologyblog.wordpress.com/2013/08/29/cyclic-cellular-automata/
https://en.wikipedia.org/wiki/Cyclic_cellular_automaton */
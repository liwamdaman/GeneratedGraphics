#pragma once
#include "Graphic.h"

#define MAX_LINES 50000	// Arbitrary limit, subject to change

#define DEFAULT_DEPTH 8
#define DEFAULT_BRANCHES_PER_NODE 2

#define MAIN_STEM_LENGTH 0.5
#define STEM_LENGTH_SHRINK_RATE 2/3
#define BASE_X_COORD 0.0
#define BASE_Y_COORD -0.7
#define INITIAL_DEPTH_LEVEL 0

#define TOTAL_SPREAD_ANGLE 50	// in degrees


class FractalTree: public Graphic
{
public:
	FractalTree(unsigned int depth, unsigned int branchesPerNode);
	FractalTree(); // Default to depth of ten and 2 branches per parent
	void CreateBranches(unsigned int depth, unsigned int currDepth, unsigned int branchesPerNode,
						std::vector<float>* vertexDataBufferPtr, std::vector<unsigned int>* indexBufferPtr,
						float startX, float startY, float distFromStart, float angle,
						float* endXPtr, float* endYPtr);
	void SetupGrid(unsigned int height, unsigned int width);
};
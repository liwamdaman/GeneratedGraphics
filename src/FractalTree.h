#pragma once
#include "Graphic.h"

#define DEFAULT_DEPTH 10
#define MAIN_STEM_LENGTH 0.5
#define STEM_LENGTH_SHRINK_RATE 2/3
#define BRANCH_SPREAD_ANGLE 25	// in degrees

class FractalTree: public Graphic
{
public:
	FractalTree(); // Default to depth of ten and 2 branches per parent
	FractalTree(unsigned int depth, unsigned int branchNum);
	void CreateBranches(unsigned int depth, unsigned int currDepth, 
						std::vector<float>* positionBuffer, std::vector<unsigned int>* indexBuffer,
						float startX, float startY, float distFromStart, float angle,
						float* endXPtr, float* endYPtr);
};
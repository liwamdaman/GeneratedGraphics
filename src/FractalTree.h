#pragma once
#include "Graphic.h"

class FractalTree: public Graphic
{
public:
	FractalTree(); // Default to depth of ten and 2 branches per parent
	FractalTree(unsigned int depth, unsigned int branchNum);

};
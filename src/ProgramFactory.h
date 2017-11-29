#pragma once
#include "programs/Program.h"

class ProgramFactory
{
public:
	static Program* CreateProgram(uint8_t num);
	static Program* CreateDefaultProgram();
	static int GetProgramCount();
};


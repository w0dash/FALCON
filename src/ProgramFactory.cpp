#include "ProgramFactory.h"
#include "programs/SoftTwinkle.h"
#include "programs/Rainbow.h"
#include "programs/Sinelon.h"
#include "programs/Rgb.h"
#include "programs/Beatwave.h"
#include "programs/Juggle.h"

Program* ProgramFactory::CreateProgram(uint8_t num)
{
	switch (num)
	{
	case 0:
		return new SoftTwinkle();
	case 1:
		return new Rainbow();
	case 2:
		return new Sinelon();
	case 3:
		return new Rgb();
	case 4:
		return new Beatwave();
	case 5:
		return new Juggle();
	default:
		CreateDefaultProgram();
	}
}

Program* ProgramFactory::CreateDefaultProgram()
{
	return new SoftTwinkle();
}

int ProgramFactory::GetProgramCount()
{
	return 6;
}

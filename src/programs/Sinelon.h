#pragma once
#include "Program.h"
class Sinelon :
	public Program
{
private:
	uint8_t gHue = 0;
public:
	Sinelon();
	void Init(uint8_t params[3]);
	void Animate();
	void DeInit();
	virtual ~Sinelon();
};


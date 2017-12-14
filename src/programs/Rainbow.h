#pragma once

#include "Program.h"
class Rainbow :
	public Program
{
private:
	uint8_t gHue = 0;
public:
	Rainbow();
	void Init(uint8_t params[3]);
	void Animate();
	void DeInit();
	virtual ~Rainbow();
};


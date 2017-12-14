#pragma once

#include "Program.h"
class Juggle :
	public Program
{
public:
	Juggle();
	void Init(uint8_t params[3]);
	void Animate();
	void DeInit();
	virtual ~Juggle();
};


#pragma once

#include "Program.h"
class Juggle :
	public Program
{
public:
	Juggle();
	void Init(byte params[3]);
	void Animate();
	void DeInit();
	virtual ~Juggle();
};


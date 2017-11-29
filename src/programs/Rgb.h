#pragma once

#include "Program.h"
class Rgb :
	public Program
{
private:
	int r = 255, g = 0, b = 255;

public:
	Rgb();
	void Init(byte params[3]);
	void Animate();
	void DeInit();
	virtual ~Rgb();
};


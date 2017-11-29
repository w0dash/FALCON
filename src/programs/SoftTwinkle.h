#pragma once
#include "Program.h"
class SoftTwinkle :
	public Program
{
private:
	int density = 80;

public:
	SoftTwinkle();
	void Init(byte params[3]);
	void Animate();
	void DeInit();
	virtual ~SoftTwinkle();
};


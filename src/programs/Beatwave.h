#pragma once

#include "Program.h"
class Beatwave :
	public Program
{
private:
	CRGBPalette16 currentPalette;
	CRGBPalette16 targetPalette;
	TBlendType    currentBlending;
public:
	Beatwave();
	void Init(byte params[3]);
	void Animate();
	void DeInit();
	virtual ~Beatwave();
};


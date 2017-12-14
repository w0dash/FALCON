#include "Rgb.h"

Rgb::Rgb()
{
	id = "Rgb";
	tooltip = "Custom-color mode";

	parameters[0].enabled = 0xff;
	parameters[0].name = "Red";
	parameters[0].min = 0;
	parameters[0].max = 255;

	parameters[1].enabled = 0xff;
	parameters[1].name = "Green";
	parameters[1].min = 0;
	parameters[1].max = 255;

	parameters[2].enabled = 0xff;
	parameters[2].name = "Blue";
	parameters[2].min = 0;
	parameters[2].max = 255;
}

void Rgb::Init(uint8_t param_values[3])
{
	if(param_values != nullptr)
	{
		r = param_values[0];
		g = param_values[1];
		b = param_values[2];
	}
}

Rgb::~Rgb()
{
}

void Rgb::Animate()
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i].r = r;
		leds[i].g = g;
		leds[i].b = b;
	}
}

void Rgb::DeInit()
{
}

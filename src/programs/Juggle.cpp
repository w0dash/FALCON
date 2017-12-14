#include "Juggle.h"

Juggle::Juggle()
{
	id = "Juggle";
	tooltip = "a juggle";
}

void Juggle::Init(uint8_t param_values[3])
{

}

Juggle::~Juggle()
{
}

void Juggle::Animate()
{
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy(leds, NUM_LEDS, 20);
	uint8_t dothue = 0;
	for (int i = 0; i < 8; i++) {
		leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}

void Juggle::DeInit()
{
}

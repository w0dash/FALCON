#include "Sinelon.h"

Sinelon::Sinelon()
{
	id = "Sinelon";
	tooltip = "who the fuck is sinelon";
}

void Sinelon::Init(uint8_t param_values[3])
{

}

Sinelon::~Sinelon()
{
}

void Sinelon::Animate()
{
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy(leds, NUM_LEDS, 20);
	int pos = beatsin16(13, 0, NUM_LEDS - 1);
	leds[pos] += CHSV(gHue, 255, 192);
}

void Sinelon::DeInit()
{
}

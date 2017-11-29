#include "Beatwave.h"

Beatwave::Beatwave()
{
	id = "Beatwave";
	tooltip = "a weird pulsating wave";
}

void Beatwave::Init(byte param_values[3])
{
	currentPalette = RainbowColors_p;
	currentBlending = LINEARBLEND;
}

Beatwave::~Beatwave()
{
}

void Beatwave::Animate()
{
	uint8_t wave1 = beatsin8(9, 0, 255);                        // That's the same as beatsin8(9);
	uint8_t wave2 = beatsin8(8, 0, 255);
	uint8_t wave3 = beatsin8(7, 0, 255);
	uint8_t wave4 = beatsin8(6, 0, 255);

	for (int i = 0; i<NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(currentPalette, i + wave1 + wave2 + wave3 + wave4, 255, currentBlending);
	}

	EVERY_N_MILLISECONDS(100) {
		uint8_t maxChanges = 24;
		nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
	}

	EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
		targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));
	}

}

void Beatwave::DeInit()
{
}

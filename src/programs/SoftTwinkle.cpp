#include "SoftTwinkle.h"

SoftTwinkle::SoftTwinkle()
{
	id = "SoftTwinkle";
	tooltip = "a soft twinkle";

	parameters[0].enabled = 0xff;
	parameters[0].name = "Density";
	parameters[0].min = 1;
	parameters[0].max = 255;
}

void SoftTwinkle::Init(uint8_t param_values[3])
{
	freq = 50;

	if(param_values != nullptr)
	{
		Serial.print("Density: ");
		Serial.println(param_values[0]);

		density = param_values[0];
	}

}

SoftTwinkle::~SoftTwinkle()
{
}

void SoftTwinkle::Animate()
{
	for (int i = 0; i < NUM_LEDS; i++) {
		if (!leds[i]) continue; // skip black pixels
		if (leds[i].r & 1) { // is red odd?
			leds[i] -= lightcolor; // darken if red is odd
		}
		else {
			leds[i] += lightcolor; // brighten if red is even
		}
	}

	// Randomly choose a pixel, and if it's black, 'bump' it up a little.
	// Since it will now have an EVEN red component, it will start getting
	// brighter over time.
	if (random8() < density) {
		int j = random16(NUM_LEDS);
		if (!leds[j]) leds[j] = lightcolor;
	}
}

void SoftTwinkle::DeInit()
{
}

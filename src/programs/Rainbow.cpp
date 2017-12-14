#include "Rainbow.h"

Rainbow::Rainbow()
{
	id = "Rainbow";
	tooltip = "a rainbow";
}

void Rainbow::Init(uint8_t param_values[3])
{

}

Rainbow::~Rainbow()
{
}

void Rainbow::Animate()
{
	fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void Rainbow::DeInit()
{
}

#pragma once
#include <stdint.h>

typedef struct Parameter
{
	char* name;
	uint8_t enabled = 0;
	uint8_t min = 0;
	uint8_t max = 255;
} Parameter;

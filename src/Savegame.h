#pragma once

#include <stdint.h>

typedef struct Savegame
{
	uint8_t prog_num, prog_param1, prog_param2, prog_param3, correction_r, correction_g, correction_b, brightness;
} Savegame;

Savegame* CreateSavegame();
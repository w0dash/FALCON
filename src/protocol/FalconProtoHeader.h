#pragma once

#include <stdint.h>

typedef struct FalconProtoHeader
{
	uint8_t Direction;
	uint8_t Type;
	uint8_t ID;
	uint16_t PayloadLength;
} FalconProtoHeader;

enum FalconProtoHeaderDirection : uint8_t
{
	Firmware, CurrentProgram
};

enum FalconProtoHeaderType : uint8_t
{
	Command, RawData
};

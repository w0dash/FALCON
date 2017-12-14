#pragma once

typedef struct FalconProtoHeader
{
	unsigned char Direction;
	unsigned char Type;
	unsigned char ID;
	unsigned short PayloadLength;
} FalconProtoHeader;

enum FalconProtoHeaderDirection : unsigned char
{
	Firmware, CurrentProgram
};

enum FalconProtoHeaderType : unsigned char
{
	Command, RawData
};

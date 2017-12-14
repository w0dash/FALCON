#pragma once

#include "HW.h"
#include "Parameter.h"

#include <stdint.h>

class Program
{
protected:
	uint16_t freq = 1000;
	uint64_t last_update_millis = 0;
	virtual void Animate() = 0;
	virtual void DataUpdate(char* buffer, int len);

public:
	char* id;
	char* tooltip;
	Parameter parameters[3];

	virtual void Init(int8_t param_values[3] = nullptr) = 0;
	void Run();
	virtual void DeInit() = 0;
};

inline void Program::Run(char* buffer = nullptr, int len = 0)
{
	if(millis() > last_update_millis + 1000/freq)
	{
		if(buffer != nullptr)
		{
			this->DataUpdate(buffer, len);
		}

		this->Animate();
		
		last_update_millis = millis();
	}
}

#pragma once

#include "HW.h"
#include "Parameter.h"

class Program
{
protected:
	int freq = 1000;
	unsigned long last_update_millis = 0;
	virtual void Animate() = 0;
	virtual void DataUpdate(char* buffer, int len) = 0;

public:
	char* id;
	char* tooltip;
	Parameter parameters[3];

	virtual void Init(byte param_values[3] = nullptr) = 0;
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

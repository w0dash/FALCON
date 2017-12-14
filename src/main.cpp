#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <EEPROM.h>
#include "EState.h"
#include "Wifi.h"
#include "HW.h"
#include "programs/Program.h"
#include "Savegame.h"
#include "ProgramFactory.h"

#include "secrets/WPA.h"
#include "protocol/WifiCommands.h"

int8_t paramA, paramB, paramC;

State state = PROGRAM;
State previousState;
Program* program;

Savegame* savegame;

Wifi* wifi;

int8_t* recv_buffer;

int8_t* i2c_req_buffer;

ulong i2c_req_timer = 0;

int menuPos = 5;
const int MENU_ENTRY_COUNT = 8;
String display_string = "";
String last_display_string = "";

bool up = false, down = false, ok = false;

//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, D5, D6, D7);
//LiquidCrystal lcd(D2, D3, D4, D5, D6, D7, 8);


// TODO: test
void SaveWifiConfig()
{	
	// WPA_SSID
	for (int i = 0; i < sizeof(WPA_SSID); i++)
	{
		EEPROM.write(i, WPA_SSID[i]);
	}
	
	// WPA_KEY
	for (int i = 0; i < sizeof(WPA_KEY); i++)
	{
		EEPROM.write(32 + i, WPA_KEY[i]);
	}

	EEPROM.commit();
}


extern void I2C_Send(const char* msg, uint8_t extra)
{
	Serial.print("TWI send ");
	Serial.println(msg);

	Wire.beginTransmission(0);
	Wire.write(extra);
	if(msg != nullptr)
	{
		Wire.write(msg);
	}
	Wire.endTransmission();
}

void SelectProgram(int8_t num, int8_t params[3])
{
	FastLED.clear();

	Serial.println("deleting old program");

	delete program;

	Serial.println("creating new program");

	program = ProgramFactory::CreateProgram(num);

	Serial.println("initializing new program");

	program->Init(params);

	Serial.println("Trying to create savegame");

	savegame->prog_num = num;

	if(params != nullptr)
	{
		savegame->prog_param1 = params[0];
		savegame->prog_param2 = params[1];
		savegame->prog_param3 = params[2];
	}
	else
	{
		//TODO: save default param values for every program in savegame
	}
}

void StoreSavegame()
{
	EEPROM.begin(8);
	
	// Doesnt work, y tho?
	EEPROM.put(0, *savegame);

	/*
	uint8_t* data = (uint8_t*)savegame;
	for (int i = 0; i < sizeof(*savegame); i++)
	{
		EEPROM.write(i, data[i]);
	}
	*/

	// overwrite last uint8_t with brightness; subject to change
	EEPROM.write(7, FastLED.getBrightness());

	EEPROM.commit();
	EEPROM.end();
}

void GetSavegameRAW(uint8_t* buffer)
{
	uint8_t* data = (uint8_t*)savegame;
	for (int i = 0; i < sizeof(*savegame); i++)
	{
		buffer[i] = data[i];
	}
}

void LoadSavegame()
{
	EEPROM.begin(8);
	EEPROM.get(0, *savegame);
	EEPROM.end();

	uint8_t uint8_ts[8];
	GetSavegameRAW(uint8_ts);

	Serial.print("*savegame [eeprom  0-7]: ");
	for (int i = 0; i < sizeof(uint8_ts); i++)
	{
		Serial.print((uint8_t)uint8_ts[i]);
		Serial.print(" ");
	}
	Serial.println();

	Serial.println("Trying to select program");

	uint8_t params[] = { savegame->prog_param1, savegame->prog_param2, savegame->prog_param3 };
	SelectProgram(savegame->prog_num, params);

	Serial.println("Trying to set brightness");

	FastLED.setCorrection(CRGB(savegame->correction_r, savegame->correction_g, savegame->correction_b));

	FastLED.setBrightness(savegame->brightness);
}

void WifiStuff()
{
	wifi->UdpDiscovery();

	int recv_bytecount;
	wifi->TcpReceive(recv_buffer, recv_bytecount);

	if (recv_bytecount > 0)
	{
		if (recv_buffer[0] == CMD_ON_OFF)
		{
			if (recv_buffer[1] == 0x0)
			{
				previousState = state;
				state = OFF;
			}
			if (recv_buffer[1] == 0x1)
			{
				state = previousState;
			}
		}

		if (recv_buffer[0] == CMD_SET_BRIGHTNESS)
		{
			FastLED.setBrightness(recv_buffer[1]);

			savegame->brightness = recv_buffer[1];
		}

		if (recv_buffer[0] == CMD_SET_PROGRAM)
		{
			state = PROGRAM;

			uint8_t params[3] = {recv_buffer[2], recv_buffer[3], recv_buffer[4]};

			SelectProgram(recv_buffer[1], params);

			FastLED.clear();
		}

		if (recv_buffer[0] == CMD_SET_COLOR_CALIBRATION)
		{
			FastLED.setCorrection(CRGB(recv_buffer[1], recv_buffer[2], recv_buffer[3]));

			savegame->correction_r = recv_buffer[1];
			savegame->correction_g = recv_buffer[2];
			savegame->correction_b = recv_buffer[3];

		}

		if (recv_buffer[0] == CMD_SET_EMULATION_MODE)
		{
			CRGB temperature = TypicalLEDStrip;

			switch(recv_buffer[1])
			{
			case 0:
				temperature = Candle;
				break;
			case 1:
				temperature = Tungsten40W;
				break;
			case 2:
				temperature = Tungsten100W;
				break;
			case 3:
				temperature = Halogen;
				break;
			case 4:
				temperature = CarbonArc;
				break;
			case 5:
				temperature = HighNoonSun;
				break;
			case 6:
				temperature = DirectSunlight;
				break;
			case 7:
				temperature = OvercastSky;
				break;
			case 8:
				temperature = ClearBlueSky;
				break;
			case 9:
				temperature = WarmFluorescent;
				break;
			case 10:
				temperature = StandardFluorescent;
				break;
			case 11:
				temperature = FullSpectrumFluorescent;
				break;
			case 12:
				temperature = GrowLightFluorescent;
				break;
			case 13:
				temperature = BlackLightFluorescent;
				break;
			case 15:
				temperature = MercuryVapor;
				break;
			case 16:
				temperature = SodiumVapor;
				break;
			case 17:
				temperature = MetalHalide;
				break;
			case 18:
				temperature = HighPressureSodium;
				break;
			
			default:
				break;
			}

			FastLED.setTemperature(temperature);
		}

		if (recv_buffer[0] == 88) 
		{
			String msg = "";
			for (int i = 1; i < RECV_BUFFER_SIZE; i++)
			{
				char c = recv_buffer[i];

				if(c == '\0')
				{
					break;
				}

				msg += c;
			}
			
			if(msg.length() > 0)
			{
				Wire.beginTransmission(0);
				Wire.write(msg.c_str());
				Wire.endTransmission();
			}
		}


		if (recv_buffer[0] == 89)
		{
			//TODO: send prog infos

			int progCount = ProgramFactory::GetProgramCount();

			uint8_t dummyParams[3];

			for (int i = 0; i < progCount; i++)
			{
				Serial.print("Trying to create prog ");
				Serial.println(i);

				String msg = "";
				Program* prog = ProgramFactory::CreateProgram(i);

				Serial.print("Created prog ");
				Serial.println(i);

				prog->Init(dummyParams);

				msg += "prog";
				msg += ";";
				msg += i;
				msg += ";";
				msg += String(prog->id);
				msg += ";";
				msg += String(prog->tooltip);
				msg += ";";

				for (int p = 0; p < 3; p++)
				{
					Serial.print("checking param ");
					Serial.println(p);

					if(prog->parameters[p].enabled)	
					{
						msg += String(prog->parameters[p].name);
						msg += ";";
						msg += prog->parameters[p].min;
						msg += ";";
						msg += prog->parameters[p].max;
						msg += ";";
					}
				}

				wifi->TcpSendMsg(msg.c_str());
				
				Serial.print("Trying to delete prog ");
				Serial.println(i);

				delete prog;
			}
		}

		if (recv_buffer[0] == CMD_SAVE_TO_EEPROM) //128
		{
			StoreSavegame();
			Serial.println("storing savegame...");
		}

		if (recv_buffer[0] == CMD_REBOOT) //255
		{
			I2C_Send(nullptr, 0xff);
			ESP.restart();
		}
	}
}

void EEPROM_TestWrite()
{
	uint8_t putput[] = {
		0x1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};

	EEPROM.begin(128);
	EEPROM.put(0, putput);

	// subject to change
	EEPROM.write(7, 32);

	EEPROM.commit();
	EEPROM.end();
}

void setup()
{
	Serial.begin(115200);

	while (!Serial)
	{
		delay(100);
	}

	Serial.println("\n");
	Serial.println("- init -");
	Serial.println("hello world!");

	pinMode(STATUS_PIN, OUTPUT);

	pinMode(BTN_OK_PIN, INPUT_PULLUP);
	pinMode(BTN_UP_PIN, INPUT_PULLUP);
	pinMode(BTN_DOWN_PIN, INPUT_PULLUP);

	digitalWrite(STATUS_PIN, LOW);

	FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.setBrightness(123);

	set_max_power_in_volts_and_milliamps(5, 500);

	Serial.println("Trying to init i2c bus");

	Wire.begin(SDA_PIN, SCL_PIN);

	I2C_Send("greetz from master", 0);

	//EEPROM_TestWrite();

	savegame = CreateSavegame();

	LoadSavegame();

	//SelectProgram(0xFF, nullptr);

	Serial.println("Jump wifi init");

	wifi = new Wifi(WPA_SSID, WPA_KEY);

	I2C_Send("wifi connected", 0);

	recv_buffer = new uint8_t[RECV_BUFFER_SIZE];

	i2c_req_buffer = new uint8_t[3];

	Serial.println("- init done -");
	Serial.println();

	Serial.flush();
}

void WifiSettingsMenu()
{
	I2C_Send("Line 1", 0);
	I2C_Send("Line 2", 1);
	delay(5000);
}

void MenuStuff()
{
	//check buttons
	if(digitalRead(BTN_UP_PIN) == LOW && !up)
	{
		menuPos++;
		up = true;
	}
	else if(digitalRead(BTN_UP_PIN) == HIGH)
	{
		up = false;
	}

	if (digitalRead(BTN_DOWN_PIN) == LOW && !down)
	{

		menuPos--;
		down = true;
	}
	else if (digitalRead(BTN_DOWN_PIN) == HIGH)
	{
		down = false;
	}

	if (digitalRead(BTN_OK_PIN) == LOW && !ok)
	{
		if(menuPos < 6)
		{
			SelectProgram(menuPos, nullptr);
		}

		switch(menuPos)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			SelectProgram(menuPos, nullptr);
			break;

		case 6:
			I2C_Send(nullptr, 0xff);
			ESP.restart();

		case 7:
			WifiSettingsMenu();

		default:
			break;
		}


		ok = true;
	}
	else if (digitalRead(BTN_OK_PIN) == HIGH)
	{
		ok = false;
	}

	if(menuPos < 0)
	{
		menuPos = MENU_ENTRY_COUNT - 1;
	}
	else if(menuPos > MENU_ENTRY_COUNT - 1)
	{
		menuPos = 0;
	}

	switch(menuPos)
	{
	case 0:
		display_string = "SoftTwinkle";	
		break;
	case 1:
		display_string = "Rainbow";
		break;
	case 2:
		display_string = "Sinelon";
		break;
	case 3:
		display_string = "RGB";
		break;
	case 4:
		display_string = "BeatWave";
		break;
	case 5:
		display_string = "Juggle";
		break;
	case 6:
		display_string = "REBOOT";
		break;
	case 7:
		display_string = "SETUP";
		break;
	default:
		display_string = "error"; 
		break;
	}

	if(!last_display_string.equals(display_string))
	{
		I2C_Send(display_string.c_str(), 0);
		last_display_string = display_string;
	}
}

void loop()
{
	WifiStuff();

	MenuStuff();

	//FSM
	switch (state)
	{
	case OFF:
		FastLED.clear(true);
		break;
	case PROGRAM:
		program->Run();
		break;
	default:
		//FastLED.clear(true);
		break;
	}

	//FastLED.show();

	FastLED.delay(20);
}

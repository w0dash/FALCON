#include "Wifi.h"
#include "HW.h"

Wifi::Wifi(const char* WPA_SSID, const char* WPA_KEY) : server(PORT)
{
	// Connect to WiFi network
	Serial.print("\nConnecting to ");
	Serial.println(WPA_SSID);

	WiFi.begin(WPA_SSID, WPA_KEY);

	int status = 1;
	int x = 0;
	while (WiFi.status() != WL_CONNECTED) {
		for (int i = 0; i < x; i++)
		{
			leds[i] = CRGB::Red;
		}
		FastLED.show();

		digitalWrite(STATUS_PIN, status);
		status ^= 1;
		Serial.print(".");
		if (x++ > NUM_LEDS - 1)
		{
			FastLED.clear();
			x = 1;
		}
		delay(50);
	}

	FastLED.clear();
	delay(167);
	FastLED.showColor(CRGB::Green);
	delay(167);
	FastLED.clear(true);
	delay(167);
	FastLED.showColor(CRGB::Green);
	delay(167);
	FastLED.clear(true);

	digitalWrite(STATUS_PIN, LOW);

	Serial.println("");
	Serial.println("WiFi connected");

	// Start the server
	//server = WiFiServer(PORT);
	server.begin();
	Serial.println("Server started");

	// Print the IP address
	Serial.print("IP: ");
	Serial.print(WiFi.localIP());
	Serial.println();

	udp.begin(PORT);

}

Wifi::~Wifi()
{
}

void Wifi::TcpReceive(uint8_t recv_buffer[], int& recv_uint8_ts)
{
	//network
	// Check if a client has connected
	if (!client || !client.connected())
	{
		client = server.available();
		//digitalWrite(STATUS_PIN, HIGH);
	}

	if (client.connected())
	{
		digitalWrite(STATUS_PIN, HIGH);
	}
	else {
		digitalWrite(STATUS_PIN, LOW);
	}

	/*
	if (!client) {
	return;
	}
	*/
	// Wait until the client sends some data
	//Serial.println("new client");

	/*
	while(!client.available()){
	delay(1);
	}
	*/

	// Read the first line of the request

	if (client)
	{
		recv_uint8_ts = client.available();

		if (recv_uint8_ts > 0)
		{
			for (int i = 0; i < recv_uint8_ts; i++)
			{
				recv_buffer[i] = client.read();
			}

			Serial.print("msg:");

			for (int i = 0; i < recv_uint8_ts; i++)
			{
				Serial.print(" ");
				Serial.print(recv_buffer[i]);
			}

			Serial.println();

			client.flush();
		}
	}
}

void Wifi::TcpSendMsg(const char* msg)
{
	//TODO: test
	if (client)
	{
		Serial.print("Trying to send tcp msg \"");
		Serial.print(msg);
		Serial.println("\"");

		client.write_P(msg, strlen(msg));
		client.flush();
	}
}


void Wifi::UdpDiscovery()
{
	/*
	IPAddress marvin(192,168,0,24);
	udp.beginPacket(marvin, 1337);
	udp.write("lam_ip ");
	udp.println(WiFi.localIP());
	udp.endPacket();
	*/

	int buffersize = udp.parsePacket();
	if (buffersize > 0)
	{
		String msg = "";
		uint8_t buffer[buffersize];
		for (int i = 0; i < buffersize; i++)
		{
			buffer[i] = udp.read();
		}

		for (int i = 0; i < buffersize; i++)
		{
			msg += char(buffer[i]);
		}

		Serial.println("UDP: " + msg);

		if (msg == "lam_discovery")
		{
			udp.beginPacket(udp.remoteIP(), PORT);
			udp.write("lam_id ");
			udp.write("drahtlos-peter");
			//udp.println(WiFi.localIP());
			udp.endPacket();
			Serial.println("UDP: id sent");
		}

		if (msg == "lam_get_proglist")
		{
			udp.beginPacket(udp.remoteIP(), PORT);
			udp.write("lam_prog SoftTwinkle,Rainbow,Sinelon,Rgb");
			//udp.println(WiFi.localIP());
			udp.endPacket();
			Serial.println("UDP: progs sent");
		}
	}
}

#pragma once

#define PORT 1337

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

class Wifi
{
	WiFiServer server;
	WiFiClient client;
	WiFiUDP udp;

public:
	Wifi(const char* WPA_SSID, const char* WPA_KEY);
	~Wifi();
	void TcpReceive(uint8_t recv_buffer[], int& recv_buffer);
	void TcpSendMsg(const char* msg);
	void UdpDiscovery();
};


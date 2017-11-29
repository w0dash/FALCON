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
	Wifi(const char* ssid, const char* key);
	~Wifi();
	void TcpReceive(byte recv_buffer[], int& recv_bytes);
	void TcpSendMsg(const char* msg);
	void UdpDiscovery();
};


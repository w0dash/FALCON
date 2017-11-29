#pragma once

//TODO: fix timing problems to remove this bullshit
#define FASTLED_ALLOW_INTERRUPTS 0

#include <Wire.h>

/*
#include <SPI.h>
#include <LiquidCrystal.h>
#include <U8g2lib.h>
*/
#include <FastLED.h>

#define SCL_PIN				D1
#define SDA_PIN				D2
#define STATUS_PIN			D0
#define LED_PIN				D8
#define LED_TYPE			WS2812B
#define COLOR_ORDER			GRB
#define NUM_LEDS			139
#define RECV_BUFFER_SIZE	32
#define I2C_REQ_INTERVAL	100

#define BTN_OK_PIN			D4
#define BTN_UP_PIN			D5
#define BTN_DOWN_PIN		D6

extern CRGB leds[];
const CRGB lightcolor(8, 7, 1);

//extern U8G2_ST7920_128X64_1_SW_SPI u8g2;
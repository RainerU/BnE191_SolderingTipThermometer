/*
github: RainerU/BnE191_SolderingTipThermometer

Arduino sketch for a simple thermometer that replaces a HAKKO 191.
It implements smart max and hold modes which are automatically selected.

Idea:
If you are measuring the temperature of your soldering iron, you are usually only interested
in the maximum value. This is because every smaller value is just related to a bad contact
of the soldering tip to the thermocouple. The sensor can get too cold, but not too hot.
Having this in mind, I created a smart software, that shows the maximum temperature. Only if
it falls back a certain amount, the SW assumes that the iron is removed and it switches into the
hold mode. In this mode, still the maximum value is displayed as before.
If now the temperature starts to rise again, the program switches back to the max mode, but
in this moment the displayed max value is reset to the current value. So you can start a new
measurement.
If the temperature falls below 50 degree C, no max or hold mode is applied but just the current
(room-) temperature is displayed. This is useful to see that the thermometer is working well.

The hardware is based on an Arduino Nano. Others are possible of course, nearly no limitations.
The thermocouple is connnected via a MAX6675 breakout.
The display is a 128x32 OLED with a SSD1306 contoller. All this is easy to modify.
I used the case of a cheap HAKKO 191 clone from China, which was not working well, but the case
is good.
Wiring is trivial. Just connect power (I used a 9V battery and the 5V regulator of the arduino)
and make the signal connections that you can find in the #define statements.

Copyright (c) 2019 Rainer Urlacher
published under the MIT License, see github repository

*/

#include <U8g2lib.h>
#include <max6675.h>

// display wiring
#define display_data A4
#define display_clock A5

// display constructor for a 128x32 OLED with SSD1306
U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C u8g2(U8G2_R0, display_clock, display_data);

// MAX6675 wiring
#define max6675_CLK 3
#define max6675_CS 4
#define max6675_DO 5

// MAX6675 constructor
MAX6675 thermocouple(max6675_CLK, max6675_CS, max6675_DO);

#define stbTemperature 50          // below this temperature is standby mode, above is max/hold mode
#define thresholdTemperature 10    // if temperature falls this value in max mode, enable hold mode

// global variables
char mode = 0; // 0 = standby, 1 = max, 2 = hold
int holdTemperature = 0;
int lastTemperature = 1000;

void setup(void) {
	// initialize display
	u8g2.begin();
	
	// LED for activity indication
	pinMode(LED_BUILTIN, OUTPUT);

	// boot-message
	u8g2.firstPage();
	do {
		u8g2.setFont(u8g2_font_courB18_tr);
		u8g2.drawStr(20,26,"BnE191");
	} while ( u8g2.nextPage() );
	delay(2000);
};

void loop(void) {
	digitalWrite(LED_BUILTIN, HIGH);

	// read thermocouple
	int temperature = thermocouple.readCelsius();
	
	// mode selection state-machine
	if (mode == 0) { // standby mode
		if (temperature > stbTemperature) mode = 1;
	} else if (mode == 1) { // max mode
		if (temperature <= stbTemperature) mode = 0;
		else if (temperature < holdTemperature - thresholdTemperature) mode = 2;
	} else { // hold mode
		if (temperature <= stbTemperature) mode = 0;
		else if (temperature > (lastTemperature+2)) {
			mode = 1;
			holdTemperature = temperature; // when switching back to max mode, start with new value
		}
	}
	
	// action for standby mode, no holding, no max
	if (mode == 0) holdTemperature = temperature;
	
	// action for max mode
	if (mode == 1) {
		if (temperature > holdTemperature) holdTemperature = temperature;
	}
	
	// store temperature for next loop
	lastTemperature = temperature;

	// display holdTemperature
	u8g2.firstPage();  // use page buffer mode of U8g2 library
	do {
		char formatted[3];
		sprintf(formatted, "%3d", holdTemperature); // format temperature for 3 digits
		u8g2.setFont(u8g2_font_courB24_tn);
		u8g2.drawStr(0,31,formatted);
		if (mode == 1) { // max mode
			u8g2.setFont(u8g2_font_courB18_tr);
			u8g2.drawStr(85,21,"max");
		} else if (mode == 2) { // hold mode
			u8g2.setFont(u8g2_font_courB18_tr);
			u8g2.drawStr(69,26,"hold");
		};
	} while ( u8g2.nextPage() );
	
	digitalWrite(LED_BUILTIN, LOW);
	delay(500);
};


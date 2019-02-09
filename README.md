# BnE191_SolderingTipThermometer
Arduino sketch for a simple thermometer that replaces a HAKKO 191 and implements smart max and hold modes.

# Idea
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

# Hardware
The hardware is based on an Arduino Nano. Others are possible of course, nearly no limitations.
The thermocouple is connnected via a MAX6675 breakout.

The display is a 128x32 OLED with a SSD1306 contoller. All this is easy to modify.

I used the case of a cheap HAKKO 191 clone from China, which was not working well, but the case
is good.

Wiring is trivial. Just connect power (I used a 9V battery and the 5V regulator of the arduino)
and make the signal connections that you can find in the #define statements.

# Video Reference
presentation on the German channel "Elektrobits": https://youtu.be/mf1f0h-3HEA

presentation on the English channel "Bits and Electrons": https://youtu.be/n-b3tQhh6aY

# License
Copyright (c) 2019 Rainer Urlacher

published under the MIT License

/****************************************************************************************************************************
  multiFileProject.ino
  For ESP32_S2 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32_S2_ISR_Servo
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#if !( ARDUINO_ESP32S2_DEV || ARDUINO_FEATHERS2 || ARDUINO_ESP32S2_THING_PLUS || ARDUINO_MICROS2 || \
       ARDUINO_METRO_ESP32S2 || ARDUINO_MAGTAG29_ESP32S2 || ARDUINO_FUNHOUSE_ESP32S2 || \
       ARDUINO_ADAFRUIT_FEATHER_ESP32S2_NOPSRAM || ARDUINO_ADAFRUIT_QTPY_ESP32S2)
  #error This code is intended to run on the ESP32_S2 platform! Please check your Tools->Board setting.
#endif

#define ESP32_S2_ISR_SERVO_VERSION_MIN_TARGET      "ESP32_S2_ISR_Servo v1.2.0"
#define ESP32_S2_ISR_SERVO_VERSION_MIN             1002000

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP32_S2_ISR_Servo.h"

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);
  
	Serial.println("\nStart multiFileProject");
	Serial.println(ESP32_S2_ISR_SERVO_VERSION);

#if defined(ESP32_S2_ISR_SERVO_VERSION_MIN)

	if (ESP32_S2_ISR_SERVO_VERSION_INT < ESP32_S2_ISR_SERVO_VERSION_MIN)
	{
		Serial.print("Warning. Must use this example on Version equal or later than : ");
		Serial.println(ESP32_S2_ISR_SERVO_VERSION_MIN_TARGET);
	}

#endif
}

void loop()
{
	// put your main code here, to run repeatedly:
}

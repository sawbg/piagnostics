#ifndef listen_H
#define listen_H

#include <unistd.h>
#include <wiringPi.h>

/*
 *
 */
void* listen(void* givenChanges) {
	const unsigned char PIN = 4;  // pin to read
	bool buttonValue;  // button value (unpressed = true, pressed = false)
	pinMode(PIN, INPUT);  // initialize pin
	
	while(true) {
		buttonValue = digitalRead(PIN);
		while(!buttonValue);  // hold until button released
		givenChanges->units = !buttonValue;
		givenChanges->lang = !buttonValue;
		usleep(100);  // so it's not constantly polling
	}

	return nullptr;
}

#endif

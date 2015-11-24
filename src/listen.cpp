#ifndef listen_H
#define listen_H

#include <unistd.h>
#include <wiringPi.h>

#include "Changes.cpp"

namespace piagnostics {

/**
 * Listens for the button to be pressed and notifies a Change instance when this
 * occurs.
 *
 * The pin to read is hard-coded.
 *
 * This function is designed to be run on a separate thread. Though I realize
 * this isn't technically thread-safe (no mutexes), it does not modify any
 * memory locations on the main thread. Therefore, it shouldn't really cause any
 * problems. Additionally, the thread never exits, because it is designed to run
 * the entire time the main() function of Piagnostics is running.
 *
 * Note that a second button press immeadiately following the first (<1 sec
 * later) is not effective.
 *
 * @param givenChanges a (void) pointer to the Changes class instance for which
 * to record changes in the language and system of units.
 *
 * @return a void pointer to the pthread_t structure for this thread
 */
void* listen(void* givenChanges) {
	const unsigned char PIN = 4;  // pin to read
	bool buttonValue;  // button value (unpressed = true, pressed = false)
	pinMode(PIN, INPUT);  // initialize pin

	while(true) {
		buttonValue = digitalRead(PIN);  // read the button value

		/* 
		 * Hold until the button is released and hope there's no
		 * bouncing
		 */
		while(!buttonValue);

		/* 
		 * Set the language and units based on the button value. Assume
		 * that the moment that Imperial units will always beused with
		 * English and metric units with German.
		 */
		((Changes*)givenChanges)->units = !buttonValue;
		((Changes*)givenChanges)->lang = !buttonValue;
		usleep(100);  // so it's not constantly polling
	}

	return nullptr;
}
}

#endif

#include <lcd.h>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <wiringPi.h>

#include "change.cpp"
#include "listen.cpp"

/**
 * The main() function of Piagnostics.
 *
 * @return means nothing
 */
int main() {
	// starts the LCD
	LCD lcd;	
	lcd.Show("LOADING", "PIAGNOSTICS...");

	// variable initialization
	string line1;
	string line2;
	string line3;
	string line4;
	Changes changes;  // holds changes in language and units
	DiagnosticAdapter adapter;
	Timer timer(5);  // hard-coded 5 sec for timer

	 // Starts thread to poll for button push.
	pthread_t lthread;
	int res = pthread_create(&lthread, NULL, listen, (void*)&changes);

	// run forever and ever
	while(true) {
		timer.Begin();  // starts timer

		while(timer) {
			change(adapter, changes);  // check for change
			line1 = adapter.Speed() + ", " + adapter.Rpm();
			line2 = adapter.FuelRate();
			line3 = adapter.MinutesSinceStart();
			line4 = adapter.CheckEngineLight();
			lcd.Show(line1, line2, line3, line4);
		}

		timer.Begin();  // restarts timer

		while(timer) {
			change(adapter, changes);
			line1 = "yo";// inside
				line2 = adapter.OutsideTemperature();
			line3 = adapter.OilTemperature();
			line4 = adapter.CoolantTemperature();
			lcd.Show(line1, line2, line3, line4);
		}

		timer.Begin();

		while(timer) {
			change(adapter, changes);
			line1 = adapter.BarometricPressure();
			line2 = adapter.IntakeManifoldPressure();
			line3 = adapter.EngineLoad();
			line4 = adapter.MilesSinceCodesCleared();
			lcd.Show(line1, line2, line3, line4);
		}

		timer.Begin();

		while(timer) {
			change(adapter, changes);
			line1 = adapter.ThrottlePosition();
			line2 = adapter.TimingAdvance();
			line3 = adapter.FuelAirRatio();
			lcd.Show(line1, line2, line3, line4);
		}
	}	
}

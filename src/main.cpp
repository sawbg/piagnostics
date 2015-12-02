#include <lcd.h>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <wiringPi.h>

#include "change.cpp"
#include "Changes.cpp"
#include "DiagnosticAdapter.cpp"
#include "Language.cpp"
#include "LCD.cpp"
#include "listen.cpp"
#include "Timer.cpp"
#include "Units.cpp"

using namespace piagnostics;

/**
 * 
 */
int main() {
	LCD lcd;
	lcd.Show("LOADING", "PIAGNOSTICS...");

	std::string line1;
	std::string line2;
	std::string line3;
	std::string line4;
	Changes changes;
	DiagnosticAdapter adapter(English, Units::Imperial);
	Timer timer(5);

	/*
	 * Starts thread to poll for button push.
	 */
	pthread_t lthread;
	int res = pthread_create(&lthread, NULL, listen, (void*)&changes);

	while(true) {
		timer.Begin();

		while(timer) {
			change(adapter, changes);
			line1 = adapter.Speed() + ", " + adapter.Rpm();
			line2 = adapter.FuelRate();
			line3 = adapter.MinutesSinceStart();
			line4 = adapter.CheckEngineLight();
			lcd.Show(line1, line2, line3, line4);
		}

		timer.Begin();

		while(timer) {
			change(adapter, changes);
			line1 = "yo";// inside
				line2 = adapter.OutsideTemperature();
			line3 = "hey";//adapter.OilTemperature();
			line4 = adapter.CoolantTemp();
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

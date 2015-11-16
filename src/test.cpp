#include <iostream>
#include <string>

#include "DiagnosticAdapter.cpp"
#include "Language.cpp"
#include "Pid.cpp"
#include "Units.cpp"
#include "UsbConnection.cpp"

using namespace piagnostics;

/**
 * Proof of concept application to be run from either a PC or a Raspberry Pi.
 *
 * @return always 0
 */
int main() {
	DiagnosticAdapter diag(English, Units::Imperial);

	for(int i = 0; i <= 1; i++) {
		cout << diag.Speed() << endl;
		cout << diag.Rpm() << endl;
		cout << diag.OutsideTemperature() << endl;
		cout << diag.CheckEngineLight() << endl;
		cout << diag.BarometricPressure() << endl;
		cout << diag.CoolantTemp() << endl;
		cout << diag.EngineLoad() << endl;
		cout << diag.FuelAirRatio() << endl;
		cout << diag.IntakeManifoldPressure() << endl;
		cout << diag.MilesSinceCodesCleared() << endl;
		cout << diag.MinutesSinceStart() << endl;
		cout << diag.ThrottlePosition() << endl;
		cout << diag.TimingAdvance() << endl;
		cout << endl;

		diag.ToggleLanguage();
		diag.ToggleUnits();
	}

	return 0;
}

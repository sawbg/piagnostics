#include <iostream>
#include <string>

#include "Language.cpp"
#include "Pid.cpp"
#include "DiagnosticAdapter.cpp"
#include "Units.cpp"
#include "UsbConnection.cpp"

using namespace piagnostics;

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
		cout << diag.FuelPressure() << endl;
		cout << diag.FuelSystemStatus() << endl;
		cout << diag.IntakeManifoldPressure() << endl;
		cout << diag.MilesSinceCodesCleared() << endl;
		cout << diag.OilTemperature() << endl;
		cout << diag.SecondsSinceStart() << endl;
		cout << diag.ThrottlePosition() << endl;
		cout << diag.TimingAdvance() << endl;
		cout << endl;

		diag.ToggleLanguage();
		diag.ToggleUnits();
	}

	return 0;
}

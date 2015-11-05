#ifndef DiagnosticAdapter_H
#define DiagnosticAdapter_H

#include <vector>

#include "BluetoothConnection.cpp"
#include "conversions.cpp"
#include "Pid.cpp"
#include "Units.cpp"

using namespace std;

namespace piagnostics {

	class DiagnosticAdapter {

		public:
			DiagnosticAdapter(BluetoothConnection &conn);

			string CheckEngineLight();
			string CoolantTemp(Units units);
			string EngineLoad();
			uint8_t* FetchData(Pid pid);
			string FuelPressure(Units units);
			string FuelSystemStatus();
			string IntakeManifoldPressure(Units units);
			string Rpm();
			string Speed(Units units);
			string TimingAdvance();

		private:
			BluetoothConnection& conn;
	};

	string DiagnosticAdapter::CheckEngineLight() {
		uint8_t* val = FetchData(Pid::StatusSinceCleared);
		return to_status(*val >> 7);		
	}

	string DiagnosticAdapter::CoolantTemp(Units units) {
		uint8_t* val = FetchData(Pid::CoolantTemp);
		*val -= 40; // accounts for 40 degree offset
		return to_temp(*val, units);
	}

	string DiagnosticAdapter::EngineLoad() {
		uint8_t* val = FetchData(Pid::EngineLoad);
		return to_string(*val * 100. / 255) + "%";  // load must be scaled
	}

	string DiagnosticAdapter::FuelPressure(Units units) {
		uint8_t* val = FetchData(Pid::FuelPressure);
		*val *= 3;
	}

	string DiagnosticAdapter::FuelSystemStatus() {
		uint8_t* val = FetchData(Pid::FuelSystemStatus);
		string ret;

		switch(*val) {
			case 1:
				ret = "Open Loop (Low Engine Temp)";
				break;
			case 2:
				ret = "Closed Loop (No Faults)";
				break;
			case 4:
				ret = "Open Loop (Engine Load Or Fuel Cut)";
				break;
			case 8:
				ret = "Open Loop (System Failure)";
				break;
			case 16:
				ret = "Closed Loop (Some Fault)";
				break;
		}

		return ret;
	}

	uint8_t* DiagnosticAdapter::FetchData(Pid pid) {
		string mode;
		int bytes;
		string val;

		switch(pid) {
			default:
				mode = "01";
				break;
		}

		switch(pid) {
			case Pid::FuelSystemStatus:
			case Pid::Rpm:
			case Pid::AirFlowRate:
			case Pid::SecondsSinceStart:
			case Pid::MilesWithLightOn:
			case Pid::MilesSinceCodesCleared:
			case Pid::ControlModuleVoltage:
			case Pid::MinutesWithLightOn:
			case Pid::MinutesSinceCodesCleared:
			case Pid::FuelRate:
				bytes = 2;
				break;

			case Pid::PidsSupported0:
			case Pid::PidsSupported20:
				bytes = 4;
				break;

			default:
				bytes = 1;
				break;
		}

		uint8_t* rsp = conn.Send(mode + to_string((int)pid) + to_string(bytes));

		return rsp;
	}
}

#endif

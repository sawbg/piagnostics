#ifndef DiagnosticAdapter_H
#define DiagnosticAdapter_H

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "conversions.cpp"
#include "Language.cpp"
#include "Pid.cpp"
#include "Units.cpp"
#include "UsbConnection.cpp"

using namespace std;

namespace piagnostics {

	class DiagnosticAdapter {

		public:
			Language lang;

			DiagnosticAdapter(Language defaultLang);

			string CheckEngineLight();
			string CoolantTemp(Units units);
			string EngineLoad();
			std::vector<uint8_t> FetchData(Pid pid);
			string FuelPressure(Units units);
			string FuelSystemStatus();
			string IntakeManifoldPressure(Units units);
			string OutsideTemperature(Units units);
			string Rpm();
			string Speed(Units units);
			string TimingAdvance();
			void ToggleLanguage();

		private:
			const char degree = 176;  // ASCII degree symbol
			UsbConnection conn;
	};

	DiagnosticAdapter::DiagnosticAdapter(Language defaultLang) {
		lang = defaultLang;
	}

	string DiagnosticAdapter::CheckEngineLight() {
		bool on = (bool)FetchData(Pid::StatusSinceCleared)[0] >> 7;
		std::string ret;

		switch(lang) {
			case English:
				ret = string("Problem Light: ") + (on ? "On" : "Off");
				break;
			case German:
				ret = string("MIL: ") + (on ? "Ein" : "Aus");
				break;
		}

		return ret;
	}

	/*string DiagnosticAdapter::CoolantTemp(Units units) {
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
	 }*/

	std::vector<uint8_t> DiagnosticAdapter::FetchData(Pid pid) {
		std::string mode;
		int bytes;
		std::string val;

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

		stringstream ss;
		ss << std::setfill('0') << std::setw(2) << std::hex << (int)pid;
		std::vector<uint8_t> resp = conn.Fetch(mode + ss.str() + to_string(bytes));
		// error code here

		resp.erase(resp.begin(), resp.begin() + 5);
		return resp;
	}

	std::string DiagnosticAdapter::OutsideTemperature(Units units) {
		short temp = (short)FetchData(Pid::OutsideTemperature)[0] - 40;
		if(units == Units::Imperial) temp = to_fahr(temp);
		return "Temp: " + to_string(temp) + degree + (units == Units::Imperial ? "F" : "C");
	}

	std::string DiagnosticAdapter::Rpm() {
		vector<uint8_t> val = FetchData(Pid::Rpm);
		short rpm = ((val[0] << 8) + val[1] + 2) / 4.; // rounds
		return to_string(rpm) + " " + (lang == English ? "RPM" : "U/min");
	}

	std::string DiagnosticAdapter::Speed(Units units) {
		short speed = (short)FetchData(Pid::Speed)[0];
		return (lang == English ? "Speed: " : "Geschw.: ")
			+ (units == Units::Imperial ? (to_string(to_mph(speed)) + " MPH")
					: (to_string(speed) + " km/h"));
	}

	void DiagnosticAdapter::ToggleLanguage() {
		lang = lang == English ? German : English;
	}

}

#endif

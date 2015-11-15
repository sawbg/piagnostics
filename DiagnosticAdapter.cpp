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

namespace piagnostics {

	class DiagnosticAdapter {

		public:
			DiagnosticAdapter(Language defaultLang, Units defaultUnits);

			std::string BarometricPressure();
			std::string CheckEngineLight();
			std::string CoolantTemp();
			std::string EngineLoad();
			std::vector<uint8_t> FetchData(Pid pid);
			std::string FuelAirRatio();			
			std::string FuelPressure();
			std::string FuelRate();
			std::string FuelSystemStatus();
			std::string IntakeManifoldPressure();
			std::string MilesSinceCodesCleared();
			std::string OilTemperature();
			std::string OutsideTemperature();
			std::string Rpm();
			std::string SecondsSinceStart();
			std::string Speed();
			std::string ThrottlePosition();
			std::string TimingAdvance();
			void ToggleLanguage();
			void ToggleUnits();
			int to_int(std::vector<unsigned char> v);

		private:
			const char ae = 132;
			const char AE = 142;
			const char degree = 176;  // ASCII degree symbol
			const char oe = 148;
			const char OE = 153;
			const char ue = 129;
			const char UE = 154;

			Language lang;
			Units units;

			UsbConnection conn;
	};

	DiagnosticAdapter::DiagnosticAdapter(Language defaultLang, Units defaultUnits) {
		lang = defaultLang;
		units = defaultUnits;
	}

	std::string DiagnosticAdapter::BarometricPressure() {
		int val = to_int(FetchData(Pid::BarometricPressure));
		if(units == Units::Metric) val = to_kpa(val);
		return (lang == English ? ("Bar. Pres.: " + to_string(val) + " " + "PSI")
				: ("Luftdruck: " + to_string(val) + " " + "kPa"));
	}

	std::string DiagnosticAdapter::CheckEngineLight() {
		bool on = (bool)(to_int(FetchData(Pid::StatusSinceCleared)) >> 7);
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

	std::string DiagnosticAdapter::CoolantTemp() {
		int val = to_int(FetchData(Pid::CoolantTemp));
		val -= 40; // accounts for 40 degree offset
		if(units == Units::Metric) val = to_celcius(val);
		return (lang == English ? "Coolant: " : ("K" + ue + string("hlmittel")))
			+ to_string(val) + degree  + (units == Units::Imperial ? "F" : "C");
	}

	std::string DiagnosticAdapter::EngineLoad() {
		int val = to_int(FetchData(Pid::EngineLoad)) / 255. * 100;
		return (lang == English ? "Engine Load: " : "Motorlast: ") + to_string(val) + "%";
	}

	std::string DiagnosticAdapter::FuelAirRatio() {
		int val = to_int(FetchData(Pid::FuelAirRatio));
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(3) << val;
		return (lang == English ? "Fuel/Air: " : "KL-Gemisch") + ss.str();
	}

	std::string DiagnosticAdapter::FuelPressure() {
		int val = to_int(FetchData(Pid::FuelPressure));
		val *= 3;
		if(units == Units::Metric) val = to_kpa(val);
		return (lang == English ? "Fuel PSI: " : "Kraftstoffd: ")
			+ to_string(val) + " "
			+ (units == Units::Imperial ? "PSI" : "kPa");
	}

	std::string DiagnosticAdapter::FuelRate() {
		int val = to_int(FetchData(Pid::FuelRate));
		return to_string(val);
	}

	std::string DiagnosticAdapter::FuelSystemStatus() {
		int val = to_int(FetchData(Pid::FuelSystemStatus));
		std::string ret;

		switch(val) {
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

		std::stringstream ss;
		ss << std::setfill('0') << std::setw(2) << std::hex << (int)pid;
		std::vector<uint8_t> resp = conn.Fetch(mode + ss.str() + to_string(bytes));
		// error code here

		resp.erase(resp.begin(), resp.begin() + 4);
		return resp;
	}

	std::string DiagnosticAdapter::IntakeManifoldPressure() {
		int val = to_int(FetchData(Pid::IntakeManifoldPressure));
		if(units == Units::Metric) val = to_kpa(val);
		return (lang == English ? "In Man PSI" : "Saugrohrd.: ")
			+ to_string(val) + " "
			+ (units == Units::Imperial ? "PSI" : "kPa");
	}

	std::string DiagnosticAdapter::MilesSinceCodesCleared() {
		int val = to_int(FetchData(Pid::MilesSinceCodesCleared));
		if(units == Units::Metric) val = to_kph(val);
		return (lang == English ? ("DTC clr " + to_string(val) + "mi ago")
				: ("DTC gefrt v " + to_string(val) + "km"));
	}

	std::string DiagnosticAdapter::OilTemperature() {
		int val = to_int(FetchData(Pid::OilTemp));
		val -= 40;
		if(units == Units::Metric) val = to_celcius(val);
		return (lang == English ? "Oil Temp: " : (OE + "ltemp: "))
			+ to_string(val) + degree
			+ (units == Units::Imperial ? "F" : "C");
	}

	std::string DiagnosticAdapter::OutsideTemperature() {
		int temp = to_int(FetchData(Pid::OutsideTemperature)) - 40;
		if(units == Units::Metric) temp = to_celcius(temp);
		return "Temp: " + to_string(temp) + degree + (units == Units::Imperial ? "F" : "C");
	}

	std::string DiagnosticAdapter::Rpm() {
		int val = to_int(FetchData(Pid::Rpm));
		short rpm = (val + 2) / 4.; // rounds
		return to_string(rpm) + " " + (lang == English ? "RPM" : "U/min");
	}

	std::string DiagnosticAdapter::SecondsSinceStart() {
		int val = to_int(FetchData(Pid::SecondsSinceStart));
		return (lang == English ? ("Started " + to_string(val) + "s ago")
				: ("Gestartet vor" + to_string(val) + "s"));
	}

	std::string DiagnosticAdapter::Speed() {
		int speed = to_int(FetchData(Pid::Speed));
		if(units == Units::Metric) speed = to_kph(speed);
		return (lang == English ? "Speed: " : "Geschw.: ")
			+ string(units == Units::Imperial ? " MPH" : " km/h");
	}

	std::string DiagnosticAdapter::ThrottlePosition() {
		int val = to_int(FetchData(Pid::ThrottlePosition)) / 255. * 100;
		return (lang == English ? "Throttle Pos.: " : "DrosStellung: ")
			+ to_string(val) + "%";
	}

	std::string DiagnosticAdapter::TimingAdvance() {
		int val = (to_int(FetchData(Pid::TimingAdvance)) - 128) / 2.;
		return (lang == English ? "Timing Adv.: " : (string("Fr")
					+ ue + "hz" + ue + "ndung: ")) + to_string(val) + degree;
	}

	void DiagnosticAdapter::ToggleLanguage() {
		lang = lang == English ? German : English;
	}

	void DiagnosticAdapter::ToggleUnits() {
		units = units == Units::Imperial ? Units::Metric : Units::Imperial;
	}

	int DiagnosticAdapter::to_int(std::vector<unsigned char> v) {
		std::string str;

		for(int i = 0; i < v.size(); i++) {
			str += (char)v[i];
		}

		return (int)std::stoul(str, nullptr, 16);
	}

}

#endif

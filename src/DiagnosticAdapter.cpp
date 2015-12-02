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

	/**
	 * This class is acts as a middle-man between the connection classes,
	 * BluetoothConnection (not implemented in this class) and
	 * UsbConnection.
	 *
	 * It still needs some work. At the moment, it only supports a USB
	 * connection with the OBD-II (Elm 327) device. It needs to add support
	 * for bluetooth as well as languages besides English and German.
	 * A different language class might make more sense for expansion.
	 */
	class DiagnosticAdapter {

		public:
			/**
			 * Initializes the Diagnostic adapter with the given
			 * languages and units.
			 *
			 * @param defaultLang display language
			 * @param defaultUnits display units
			 */
			DiagnosticAdapter(Language defaultLang, Units defaultUnits);

			/**
			 * Returns the barometric pressure according to the
			 * adapter's language and units in a string to be
			 * displayed on the LCD.
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD
			 */
			std::string BarometricPressure();

			/**
			 * Returns the status (on/off) of the check-engine
			 * light (i.e., the malfunction indicator lamp)
			 * specific to the class language in a string to be
			 * displayed on the LCD.
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD
			 */
			std::string CheckEngineLight();

			/**
			 * Returns the engine coolant temperature according to
			 * the adapter's language and units in a string to be
			 * displayed on the LCD.
			 *
			 * @return one-line (20 character max) string to be
			 * displayed on the LCD
			 */
			std::string CoolantTemp();

			/**
			 * Returns the engine load according to language as a
			 * string to be displayed on the LCD.
			 *
			 * @return one-line (20 character max) string to be
			 * displayed on the LCD
			 */
			std::string EngineLoad();

			/**
			 * Returns requested mode 1 diagnostic information from
			 * the USB connection as a vector of unsigned integers.
			 *
			 * This functions needs some work. It should probably be
			 * split into two functions, one that retrieves the
			 * information (the rx array) and another that converts
			 * it from its hex format in the array to a 4- or 8-bit
			 * integer. While the to_int() function does this,
			 * it would be nice to only have to call a single
			 * function to get the integer form of the data.
			 *
			 * @param pid the PID of the desired data
			 *
			 * @return a vector of unsigned, 8-bit integers
			 * containing the Elm 327 chip's reply to the PID
			 */
			std::vector<uint8_t> FetchData(Pid pid);

			/**
			 * Returns the fuel-to-air ratio of the fuel injector
			 * according to the adapter language as a string to be
			 * displayed on the LCD.
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD
			 */
			std::string FuelAirRatio();

			/**
			 * Returns the current rate of fuel consumption
			 * according to the language and units of the adapter
			 * as a string to be displayed on the LCD.
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD
			 */
			std::string FuelRate();

			/**
			 * Returns the absolute intake manifold pressure in
			 * the language and units of the adapter to be
			 * displayed on the LCD.
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD.
			 */
			std::string IntakeManifoldPressure();

			/**
			 * Returns the distance traveled according to the class
			 * language and units to be displayed on the LCD.
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD.
			 */
			std::string MilesSinceCodesCleared();

			/**
			 * Returns the minutes since the engine was started
			 * according to the adapter language to be displayed
			 * on the LCD.
			 *
			 * The OBD-II protocol specifies the RPM be in units of
			 * one-quarter rotations. Therefore, this function
			 * rounds to the nearest whole RPM and includes this
			 * value in the returned string.
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD.
			 */
			std::string MinutesSinceStart();

			/**
			 * Returns the outside (ambient) temperature in the
			 * correct language and units to be displayed on the
			 * LCD.
			 *
			 * I should note that this value has differed from the
			 * ambient temperature displayed on the dashboard of my
			 * Ford Escape by up to 10*F (5.6*C).
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD.
			 */
			std::string OutsideTemperature();

			/**
			 * Retrns the engine RPM according to the language of
			 * the adapter to be displayed on the LCD.
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD.
			 */
			std::string Rpm();
			
			/**
			 * Returns the current speed in the appropriate
			 * language and units to be displayed on the LCD.
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD.
			 */
			std::string Speed();
			
			/**
			 * Returns the relative throttle position, expressed as
			 * a percent, according to the adapter language to be
			 * displayed on the LCD.
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD.
			 */
			std::string ThrottlePosition();
	
			/**
			 * Returns the first-cylinder timing advance
			 * in degrees according to the class language to be
			 * displayed on the LCD.
			 *
			 * @return one-line (20 characters max) string to be
			 * displayed on the LCD.
			 */
			std::string TimingAdvance();
			
			/**
			 * Toggles the adapter language between English and
			 * German.
			 *
			 * This function must be removed when a third language
			 * is added.
			 */
			void ToggleLanguage();
			
			/**
			 * Toggles the adapter's system of measurement between
			 * Imperial and metric units.
			 */
			void ToggleUnits();
			
			/**
			 * Converts a vector of unsigned, 8-bit integers
			 * (presumably from the FetchData function) to a 32-bit
			 * integer.
			 *
			 * The first elements of the vector are given the
			 * greatest values (i.e., given the positions closer
			 * to the MSB of the integer).
			 *
			 * @param v vector containging up to four unsigned,
			 * 8-bit integers
			 *
			 * @return a 32-bit integer combination of the vector
			 * elements
			 */
			int to_int(std::vector<unsigned char> v);

		private:
			const char ae = 132;  // lowercase a-umlaut
			const char AE = 142;  // uppercase a-umlaut
			const char degree = 176;  // degree symbol
			const char oe = 148;  // lowercase o-umlaut
			const char OE = 153;  // uppercase o-umlaut
			const char ue = 129;  // lowercase u-umlaut
			const char UE = 154;  // uppercase u-umlaut

			/**
			 * The adapter's current langauge
			 */
			Language lang;
			
			/**
			 * The adapter's current system of measurements
			 */
			Units units;

			UsbConnection conn;  // serves no purpose as class ATM
	};

	DiagnosticAdapter::DiagnosticAdapter(Language defaultLang, Units defaultUnits) {
		lang = defaultLang;
		units = defaultUnits;
	}

	std::string DiagnosticAdapter::BarometricPressure() {
		int val = to_int(FetchData(Pid::BarometricPressure));
		if(units == Units::Imperial) val = to_psi(val);
		return (lang == English ? ("Baro Press: " + to_string(val) + " " + "PSI")
				: ("Luftdruck: " + to_string(val) + " " + "kPa"));
	}

	std::string DiagnosticAdapter::CheckEngineLight() {
		bool on = (bool)(to_int(FetchData(Pid::StatusSinceCleared)) & 0x01);
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
		if(units == Units::Imperial) val = to_fahrenheit(val);
		return (lang == English ? "Coolant: " : ("K" + ue + string("hlmittel: ")))
			+ to_string(val) + degree  + (units == Units::Imperial ? "F" : "C");
	}

	std::string DiagnosticAdapter::EngineLoad() {
		int val = to_int(FetchData(Pid::EngineLoad)) / 255. * 100;
		return (lang == English ? "Engine Load: " : "Motorlast: ") + to_string(val) + "%";
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

	std::string DiagnosticAdapter::FuelAirRatio() {
		float val = to_int(FetchData(Pid::FuelAirRatio)) / 32768.;
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(3) << val;
		return (lang == English ? "Fuel/Air: " : "KL-Gemisch: ") + ss.str();
	}

	std::string DiagnosticAdapter::FuelRate() {
		int val = to_int(FetchData(Pid::FuelRate));
		return to_string(val);
	}

	/*std::string DiagnosticAdapter::FuelSystemStatus() {
	  int val = to_int(FetchData(Pid::FuelSystemStatus)) >> 8;
	  std::string ret = "ERROR";

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
	  }*/

	std::string DiagnosticAdapter::IntakeManifoldPressure() {
		int val = to_int(FetchData(Pid::IntakeManifoldPressure));
		if(units == Units::Imperial) val = to_psi(val);
		return (lang == English ? "In Man: " : "Saugrohrd: ")
			+ to_string(val) + " "
			+ (units == Units::Imperial ? "PSI" : "kPa");
	}

	std::string DiagnosticAdapter::MilesSinceCodesCleared() {
		int val = to_int(FetchData(Pid::MilesSinceCodesCleared));
		if(units == Units::Imperial) val = to_mph(val);
		return (lang == English ? ("DTC clr " + to_string(val) + "mi ago")
				: ("DTC gefrt v " + to_string(val) + "km"));
	}

	std::string DiagnosticAdapter::MinutesSinceStart() {
		int val = (to_int(FetchData(Pid::SecondsSinceStart)) + 30) / 60;
		return (lang == English ? ("Started " + to_string(val) + "m ago")
				: ("Gestartet vor " + to_string(val) + "m"));
	}

	std::string DiagnosticAdapter::OutsideTemperature() {
		int temp = to_int(FetchData(Pid::OutsideTemperature)) - 40;
		if(units == Units::Imperial) temp = to_fahrenheit(temp);
		return "Temp: " + to_string(temp) + degree + (units == Units::Imperial ? "F" : "C");
	}

	std::string DiagnosticAdapter::Rpm() {
		int val = to_int(FetchData(Pid::Rpm));
		short rpm = (val + 2) / 4.; // rounds
		return to_string(rpm) + " " + (lang == English ? "RPM" : "U/min");
	}

	std::string DiagnosticAdapter::Speed() {
		int speed = to_int(FetchData(Pid::Speed));
		if(units == Units::Imperial) speed = to_mph(speed);
		return to_string(speed)	+ string(units == Units::Imperial ? " MPH" : " km/h");
	}

	std::string DiagnosticAdapter::ThrottlePosition() {
		int val = to_int(FetchData(Pid::ThrottlePosition)) / 255. * 100;
		return (lang == English ? "Throttle Pos: " : "DrosStellung: ")
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
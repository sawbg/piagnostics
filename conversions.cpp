#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <string>
#include <type_traits>

#include "Pid.cpp"
#include "Units.cpp"

using namespace std;

namespace piagnostics {

/*	string PidName(Pid pid) {
		return "";
	}

	uint8_t to_fahr(uint8_t temp) {
		return 1.8 * temp + 32;
	}

	string to_status(uint8_t status) {
		return ((bool)status ? "On" : "Off");
	}

	string to_temp(uint8_t temp, Units units) {
		char unit = 'C';

		if(units == Units::English) {
			temp = to_fahr(temp);
			unit = 'F';
		}

		return to_string(temp) + (char)176 + unit;
	}*/

	short to_fahr(short temp) { return 1.8 * temp + 32; }
	
	short to_mph(short kph) { return (short)(kph * 0.621371); }
}

#endif

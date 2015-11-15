#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <string>
#include <type_traits>

#include "Pid.cpp"
#include "Units.cpp"

using namespace std;

namespace piagnostics {

	short to_fahrenheit(short temp) { return 1.8 * temp + 32; }
	
	short to_mph(short kph) { return (short)(kph * 0.621371); }

	short to_psi(short kpa) { return (short)(0.145 * kpa); }
}

#endif

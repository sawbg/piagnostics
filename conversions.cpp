#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <string>
#include <type_traits>

#include "Pid.cpp"
#include "Units.cpp"

using namespace std;

namespace piagnostics {

	short to_celcius(short temp) { return (temp - 32) * 5. / 9.; }
	
	short to_kph(short mph) { return (short)(mph / 0.621371); }

	short to_kpa(short psi) { return (short)(6.89476 * psi); }
}

#endif

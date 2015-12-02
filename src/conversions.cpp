#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include "Pid.cpp"
#include "Units.cpp"

namespace piagnostics {

	/**
	 * Converts temperatures from celcius to fahrenheit.
	 *
	 * @param temp the celcius temperature
	 *
	 * @return the fahrenheit temperature
	 */
	short to_fahrenheit(short temp) { return 1.8 * temp + 32; }
	
	/**
	 * Converts a speed in kilometers per hour (KPH) to miles per hour
	 * (MPH).
	 *
	 * @param kph the speed in KPH
	 *
	 * @return the speed in MPH
	 */
	short to_mph(short kph) { return (short)(kph * 0.621371); }

	/**
	 * Converts pressures in kilopascals (kPa) to pounds per square-inch
	 * (PSI).
	 *
	 * @param kpa the pressure in kPa
	 *
	 * @return the pressure in PSI
	 */
	short to_psi(short kpa) { return (short)(0.145 * kpa); }
}

#endif

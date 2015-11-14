/*
 * This enumeration details the OBD-II Mode 1 PIDs.
 *
 */

#ifndef Pid_H
#define Pid_H

namespace piagnostics {

	enum class Pid : int {
		PidsSupported0,
		StatusSinceCleared,
		FreezeDtc,
		FuelSystemStatus,
		EngineLoad,
		CoolantTemp,
		FuelPressure = 0x0A,  // gauge
		IntakeManifoldPressure,  // absolute
		Rpm,
		Speed,
		TimingAdvance,
		IntakeAirTemp,
		AirFlowRate,
		ThrottlePosition,
		SecondsSinceStart = 0x1F,
		PidsSupported20,
		MilesWithLightOn,
		FuelRailPressure,
		MilesSinceCodesCleared = 0x31,
		BarometricPressure = 0x33,
		PidsSupported40 = 0x40,
		DriveCycleStatus,
		ControlModuleVoltage,
		AbsoluteLoad,
		FuelAirRatio,
		RelativeThrottlePosition,
		OutsideTemperature,
		MinutesWithLightOn = 0x4D,
		MinutesSinceCodesCleared,
		FuelType,
		EthanolPercent,
		RelativePedalPosition = 0x5A,
		OilTemp = 0x5C,
		FuelInjectionTiming,
		FuelRate
	};
}

#endif

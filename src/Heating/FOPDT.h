/*
 * FOPDT.h
 *
 *  Created on: 16 Aug 2016
 *      Author: David
 *
 *  Class to represent the parameters of a first order process with dead time
 */

#ifndef SRC_HEATING_FOPDT_H_
#define SRC_HEATING_FOPDT_H_

#include "RepRapFirmware.h"

// This is how PID parameters are stored internally
struct PidParameters
{
	float kP;			// controller (not model) gain
	float recipTi;		// reciprocal of controller integral time
	float tD;			// controller differential time
};

// This is how PID parameters are given in M301 commands
struct M301PidParameters
{
	float kP;
	float kI;
	float kD;
};

class FileStore;

class FopDt
{
public:
	FopDt();

	bool SetParameters(float phr, float pcr, float pcrChange, float pdt, float pMaxPwm, float temperatureLimit, float pVoltage, bool pUsePid, bool pInverted);

	// Stored parameters
	float GetHeatingRate() const noexcept { return heatingRate; }
	float GetCoolingRateFanOff() const noexcept { return coolingRateFanOff; }
	float GetCoolingRateFanOn() const noexcept { return coolingRateFanOff + coolingRateChangeFanOn; }
	float GetCoolingRateChangeFanOn() const noexcept { return coolingRateChangeFanOn; }
	float GetDeadTime() const { return deadTime; }
	float GetMaxPwm() const { return maxPwm; }
	float GetVoltage() const { return standardVoltage; }
	bool UsePid() const { return usePid; }
	bool IsInverted() const { return inverted; }
	bool IsEnabled() const { return enabled; }

	// Derived parameters
	float GetGainFanOff() const noexcept { return heatingRate/coolingRateFanOff; }
	float GetTimeConstantFanOff() const noexcept { return 1.0/coolingRateFanOff; }
	float GetTimeConstantFanOn() const noexcept { return 1.0/GetCoolingRateFanOn(); }
	bool ArePidParametersOverridden() const { return pidParametersOverridden; }
	M301PidParameters GetM301PidParameters(bool forLoadChange) const;
	void SetM301PidParameters(const M301PidParameters& params);
	void SetRawPidParameters(float p_kP, float p_recipTi, float p_tD);

	const PidParameters& GetPidParameters(bool forLoadChange) const
	{
		return (forLoadChange) ? loadChangeParams : setpointChangeParams;
	}

private:
	void CalcPidConstants();

	float heatingRate;
	float coolingRateFanOff;
	float coolingRateChangeFanOn;
	float deadTime;
	float maxPwm;
	float standardVoltage;					// power voltage reading at which tuning was done, or 0 if unknown
	bool enabled;
	bool usePid;
	bool inverted;
	bool pidParametersOverridden;

	PidParameters setpointChangeParams;		// parameters for handling changes in the setpoint
	PidParameters loadChangeParams;			// parameters for handling changes in the load
};

#endif /* SRC_HEATING_FOPDT_H_ */

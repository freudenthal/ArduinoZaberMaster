
#ifndef ZABER_DEVICE_AXIS_PAIR_HPP //check for multiple inclusions
#define ZABER_DEVICE_AXIS_PAIR_HPP

#include "Arduino.h"

#include <vector>

#define ZaberMaxDevices 3
#define ZaberMaxAxes 3

namespace Zaber
{
	struct IndexPair
	{
		uint8_t Device = 0;
		uint8_t Axis = 0;
		
		bool IsDeviceSpecific() const;
		bool IsAxisSpecific() const;
		bool SpecifiesOnlyOne() const;

		// compare with other pairs
		bool TargetsSingleAxisOf(IndexPair otherIndexPair) const;
	};

	struct ActiveIndexPairMap
	{
		uint8_t ActiveDevices = 0;
		uint8_t ActiveAxesByDevice[ZaberMaxDevices] = {};

		bool AnyActive(IndexPair indexPair) const;
		bool AnyActive(uint8_t Device, uint8_t Axis) const;
		
		// range helper
		static std::vector<uint8_t> MakeRangeVector(uint8_t startInclusive, uint8_t stopInclusive);
		static std::vector<uint8_t> AllDevicesVector();
		
		// helpers for interpreting which devices and axes are specified by a index (whether 0 -> active range or non-0 -> same value)
		std::vector<uint8_t> GetSpecifiedDeviceList(uint8_t Device) const;
		std::vector<uint8_t> GetSpecifiedAxisListForDevice(IndexPair indexPair) const;
		std::vector<uint8_t> GetSpecifiedAxisListForDevice(uint8_t Device, uint8_t Axis) const;
		std::vector<IndexPair> GetSpecifiedDeviceAxisList(IndexPair indexPair) const;
		std::vector<IndexPair> GetSpecifiedDeviceAxisList(uint8_t Device, uint8_t Axis) const;
	};
}

#endif // ZABER_DEVICE_AXIS_PAIR_HPP

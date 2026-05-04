#include "IndexPair.hpp"

#include <numeric>

namespace Zaber
{
	// Pair methods

	bool IndexPair::IsDeviceSpecific() const
	{
		return Device != 0;
	}

	bool IndexPair::IsAxisSpecific() const
	{
		return Axis != 0;
	}

	bool IndexPair::SpecifiesOnlyOne() const
	{
		return IsDeviceSpecific() && IsAxisSpecific();
	}

	bool IndexPair::TargetsSingleAxisOf(IndexPair otherIndexPair) const
	{
		return (Device == 0 || Device == otherIndexPair.Device) && (Axis == otherIndexPair.Axis);
	}

	// active indexes methods

	bool ActiveIndexPairMap::AnyActive(IndexPair indexPair) const
	{
		return AnyActive(indexPair.Device, indexPair.Axis);
	}

	bool ActiveIndexPairMap::AnyActive(uint8_t Device, uint8_t Axis) const
	{
		// cannot target active axes if device target is inactive
		if (Device > ActiveDevices)
		{
			return false;
		}

		if (Device != 0)
		{
			// if it targets only one device, the targeted axis must be active on that device
			return Axis <= ActiveAxesByDevice[Device-1];
		}
		else // Device == 0
		{
			// if it targets multiple devices, then the targeted axis must be active on at least one of the active devices
			int maxActiveAxes = 0;
			for (int deviceIndex = 1; deviceIndex < ActiveDevices && Axis > maxActiveAxes; deviceIndex++)
			{
				if (ActiveAxesByDevice[deviceIndex-1] > maxActiveAxes)
				{
					maxActiveAxes = ActiveAxesByDevice[deviceIndex-1];
				}
			}
			return Axis <= maxActiveAxes;
		}
	}

	// range helpers for interpreting which devices and axes are specified by a index (whether 0 -> active range or non-0 -> same value)

	std::vector<uint8_t> ActiveIndexPairMap::MakeRangeVector(uint8_t startInclusive, uint8_t stopInclusive)
	{
		std::vector<uint8_t> vector(stopInclusive - startInclusive + 1);
		std::iota(vector.begin(), vector.end(), startInclusive);
		return vector;
	}

	std::vector<uint8_t> ActiveIndexPairMap::AllDevicesVector()
	{
		return MakeRangeVector(1, ZaberMaxDevices);
	}

	std::vector<uint8_t> ActiveIndexPairMap::GetSpecifiedDeviceList(uint8_t DeviceRef) const
	{
		if (DeviceRef == 0)
		{
			return MakeRangeVector(1, ActiveDevices);
		}
		else
		{
			return { DeviceRef };
		}
	}

	std::vector<uint8_t> ActiveIndexPairMap::GetSpecifiedAxisListForDevice(IndexPair indexPair) const
	{
		return GetSpecifiedAxisListForDevice(indexPair.Device, indexPair.Axis);
	}

	std::vector<uint8_t> ActiveIndexPairMap::GetSpecifiedAxisListForDevice(uint8_t DeviceRef, uint8_t AxisRef) const
	{
		if (AxisRef == 0)
		{
			return MakeRangeVector(1, ActiveAxesByDevice[DeviceRef-1]);
		}
		else
		{
			return { AxisRef };
		}
	}

	std::vector<IndexPair> ActiveIndexPairMap::GetSpecifiedDeviceAxisList(IndexPair indexPair) const
	{
		return GetSpecifiedDeviceAxisList(indexPair.Device, indexPair.Axis);
	}

	std::vector<IndexPair> ActiveIndexPairMap::GetSpecifiedDeviceAxisList(uint8_t DeviceRef, uint8_t AxisRef) const
	{
		if (DeviceRef == 0)
		{
			if (AxisRef == 0)
			{
				// size is sum of AxesFound on all Devices
				uint8_t size = 0;
				for (uint8_t DeviceIndex = 0; DeviceIndex < ActiveDevices; DeviceIndex++)
					size += ActiveAxesByDevice[DeviceIndex];

				std::vector<IndexPair> pairs(size);
				for (uint8_t DeviceIndex = 1; DeviceIndex <= ActiveDevices; DeviceIndex++)
				{
					for (uint8_t AxisIndex = 1; AxisIndex <= ActiveAxesByDevice[DeviceIndex-1]; AxisIndex++)
					{
						pairs.push_back({ DeviceIndex, AxisIndex });
					}
				}
				return pairs;
			}
			else // AxisRef != 0
			{
				std::vector<IndexPair> pairs(ActiveDevices); // predicted size is above what actual size may be
				for (uint8_t DeviceIndex = 1; DeviceIndex <= ActiveDevices; DeviceIndex++)
				{
					if (AxisRef <= ActiveAxesByDevice[DeviceIndex-1])
						pairs.push_back({ DeviceIndex, AxisRef });
				}
				return pairs;
			}
		}
		else // DeviceRef != 0
		{
			if (AxisRef == 0)
			{
				std::vector<IndexPair> pairs(ActiveAxesByDevice[DeviceRef-1]);
				for (uint8_t AxisIndex = 1; AxisIndex <= ActiveAxesByDevice[DeviceRef-1]; AxisIndex++)
				{
					pairs.push_back({ DeviceRef, AxisIndex });
				}
				return pairs;
			}
			else // AxisRef != 0
			{
				return {{ DeviceRef, AxisRef }};
			}
		}
	}
}

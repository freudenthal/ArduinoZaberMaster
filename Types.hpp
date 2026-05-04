#ifndef ZABER_TYPES_HPP //check for multiple inclusions
#define ZABER_TYPES_HPP

#include "Arduino.h"

#include <optional>
#include <string_view>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

namespace Zaber
{
	enum class Scope : uint8_t 
	{
		Device,
		Axis,
		/**
		 * this applies to 3 commands:
		 * - storage [axis] append <key> <value>
		 * - storage [axis] exists <key>
		 * - storage [axis] get <key>
		 */
		DeviceOrAxis,
		/**
		 * this applies to 4 commands:
		 * - get <setting> // the scope of get and set setting are dependent on the scope of the setting key
		 * - set <setting> <value>
		 * - storage [axis] set <key> <value> // this might be mislabelled since storage get <key> is specified as Axis
		 * - warnings [clear] // warnings can be individual per axis or per device.
		 */
		DeviceAndAxis,
		/**
		 * this applies to 2 commands:
		 * - storage [axis|all] erase [<key>]
		 * - storage [axis|all] print [keys] [prefix <prefix>]
		 * note: both of these commands have three target options which other storage commands do not:
		 * to target device, axis, or both.
		 * hence "and/or"
		 */
		DeviceAndOrAxis,
	};

	// Message
	enum class ResponseType : uint8_t 
	{
		Reply,
		Info,
		Alert,
	};
	struct ResponseTypeProperties
	{
		char Identifier;
		const char* ReadableName;

		static const ResponseTypeProperties ALL_MESSAGES[];
		static const ResponseTypeProperties& Of(ResponseType type);
	};
	std::optional<ResponseType> ToResponseType(char c);

	// Status
	enum class StatusType : uint8_t 
	{
		Idle,
		Busy,
	};
	struct StatusTypeProperties
	{
		const char* Code;
		const char* ReadableName;

		static const StatusTypeProperties ALL_STATUSES[];
		static const StatusTypeProperties& Of(StatusType type);
	};
	std::optional<StatusType> ToStatusType(std::string_view codeString);

	// Flag
	enum class FlagType : uint8_t 
	{
		Okay,
		Rejected,
	};
	struct FlagTypeProperties
	{
		const char* Code;
		const char* ReadableName;

		static const FlagTypeProperties ALL_FLAGS[];
		static const FlagTypeProperties& Of(FlagType type);
	};
	std::optional<FlagType> ToFlagType(std::string_view codeString);

	// Warning
	enum class WarningType : uint8_t 
	{
		None,
		DriverDisabled,
		EncoderError,
		StalledAndStopped,
		ExcessiveTwist,
		StreamBoundErrors,
		InterpolationDeviation,
		LimitError,
		UnexpectedLimit,
		VoltageOutOfRange,
		OverTemperature,
		StationaryDisplacement,
		NoReference,
		ManualControl,
		CommandInterrupted,
		StreamDisconinuity,
		SettingsUpdating,
		JoystickCalibrating,
	};
	struct WarningTypeProperties
	{
		const char* Code;
		const char* ReadableName;
		const char* ReadableDescription;
		bool NeedsClearing;
		bool Important;

		static const WarningTypeProperties ALL_WARNINGS[];
		static const WarningTypeProperties& Of(WarningType type);
	};
	std::optional<WarningType> ToWarningType(std::string_view codeString);

	// Settings
	enum class SettingType : uint8_t 
	{
		Accel,
		CLoopCounts,
		CLoopDisplaceTolerance,
		CLoopDurationMax,
		CLoopMode,
		CLoopStallTimeout,
		CLoopSteps,
		CommAddress,
		CommAlert,
		CommCheckSum,
		CommProtocol,
		CommRS232Baud,
		CommRS232Protocol,
		CommRS485Baud,
		CommRS485Enable,
		CommRS485Protocol,
		CommUSBProtocol,
		DeviceID,
		DriverCurrentHold,
		DriverCurrentMax,
		DriverCurrentRun,
		DriverDir,
		DriverTemperature,
		EncoderCount,
		EncoderCountH,
		EncoderDir,
		EncoderError,
		EncoderFilter,
		EncoderIndexCount,
		EncoderIndexPhase,
		EncoderMode,
		EncoderPos,
		JoyDebug,
		KnobDir,
		KnobDistance,
		KnobEnable,
		KnobMaxSpeed,
		KnobMode,
		KnobSpeedProfile,
		LimitApproachMaxSpeed,
		LimitCycleDist,
		LimitDetectDecelOnly,
		LimitDetectMaxSpeed,
		LimitHomeAction,
		LimitHomeEdge,
		LimitHomePos,
		LimitHomePosUpdate,
		LimitHomePreset,
		LimitHomeState,
		LimitHomeTriggered,
		LimitHomeType,
		LimitAwayAction,
		LimitAwayEdge,
		LimitAwayPos,
		LimitAwayPosUpdate,
		LimitAwayPreset,
		LimitAwayState,
		LimitAwayTriggered,
		LimitAwayType,
		LimitCAction,
		LimitCEdge,
		LimitCPos,
		LimitCPosUpdate,
		LimitCPreset,
		LimitCState,
		LimitCTriggered,
		LimitCType,
		LimitDAction,
		LimitDEdge,
		LimitDPos,
		LimitDPosUpdate,
		LimitDPreset,
		LimitDState,
		LimitDTriggered,
		LimitDType,
		LimitMax,
		LimitMin,
		LimitStartPos,
		LimitSwapInputs,
		LockstepNumgroups,
		LockstepTolerance,
		MaxSpeed,
		MotionAccelOnly,
		MotionDecelOnly,
		PeripheralID,
		Pos,
		Resolution,
		StreamNumBufs,
		StreamNumStreams,
		SystemAccess,
		SystemAxisCount,
		SystemCurrent,
		SystemLEDEnable,
		SystemSerial,
		SystemTemperature,
		SystemVoltage,
		Version,
		VersionBuild,
		VirtualNumVirtual,
	};
	struct SettingTypeProperties
	{
		const char* Code;
		Scope scope;
		bool Writeable;

		static const SettingTypeProperties ALL_SETTINGS[];
		static const SettingTypeProperties& Of(SettingType type);
	};

	// Rejection Error
	enum class RejectionErrorType : uint8_t 
	{
		None,
		Again,
		AxisOnly,
		BadAxis,
		BadCommand,
		BadData,
		BadMessageID,
		BadSplit,
		BrakeClosed,
		CyclicOnly,
		DeviceOnly,
		DriverDisabled,
		Full,
		Inactive,
		InvalidLimits,
		LockStep,
		LongWord,
		NoAccess,
		NotSin,
		NotSupported,
		OverDriveLimit,
		Parked,
		ProcessOn,
		Remote,
		RemoteAndEnabled,
		StatusBusy,
		SystemError,
		Twisted,
	};
	struct RejectionError
	{
		const char* Code;
		const char* ReadableDescription;
		bool TryAgain;
		bool Important;

		static const RejectionError ALL_REJECTION_ERRORS[];
		static const RejectionError& Of(RejectionErrorType type);
	};
	std::optional<RejectionErrorType> ToRejectionErrorType(std::string_view codeString);
}

#endif // ZABER_TYPES_HPP

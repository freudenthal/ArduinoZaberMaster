#include "Types.hpp"

namespace Zaber
{
	// Response Type
	const ResponseTypeProperties ResponseTypeProperties::ALL_MESSAGES[]=
	{
		[(uint8_t)ResponseType::Reply] = { '@', "Reply" },
		[(uint8_t)ResponseType::Info] = { '#', "Info" },
		[(uint8_t)ResponseType::Alert] = { '!', "Alert" },
		// invalid entry
		{ '\0', "INVALID", },
	};
	const ResponseTypeProperties& ResponseTypeProperties::Of(ResponseType type)
	{
		const uint8_t COUNT = ARRAY_SIZE(ALL_MESSAGES);
		uint8_t index = (uint8_t)type;
		// if index is invalid, set index to index of invalid entry
		if (index >= COUNT)
			index = COUNT - 1;
		return ALL_MESSAGES[index];
	}
	std::optional<ResponseType> ToResponseType(char c)
	{
		if (c == '@') return ResponseType::Reply;
		if (c == '#') return ResponseType::Info;
		if (c == '!') return ResponseType::Alert;
		return std::nullopt;
	}

	// Status Type
	const StatusTypeProperties StatusTypeProperties::ALL_STATUSES[]=
	{
		[(uint8_t)StatusType::Idle] = { "IDLE", "Idle" },
		[(uint8_t)StatusType::Busy] = { "BUSY", "Busy" },
		// [(uint8_t)StatusType::Unknown] = { nullptr, "Unknown" },
		// invalid entry
		{ nullptr, "INVALID" },
	};
	const StatusTypeProperties& StatusTypeProperties::Of(StatusType type)
	{
		const uint8_t COUNT = ARRAY_SIZE(ALL_STATUSES);
		uint8_t index = (uint8_t)type;
		// if index is invalid, set index to index of invalid entry
		if (index >= COUNT)
			index = COUNT - 1;
		return ALL_STATUSES[index];
	}
	std::optional<StatusType> ToStatusType(std::string_view codeString)
	{
		for (uint8_t i = 0; i < ARRAY_SIZE(StatusTypeProperties::ALL_STATUSES); i++)
		{
			const auto& properties = StatusTypeProperties::ALL_STATUSES[i];
			if (codeString == properties.Code) return StatusType(i);
		}
		return std::nullopt;
	}

	// Flag Type
	const FlagTypeProperties FlagTypeProperties::ALL_FLAGS[] =
	{
		[(uint8_t)FlagType::Okay] = { "OK", "Okay" },
		[(uint8_t)FlagType::Rejected] = { "RJ", "Rejected" },
		// invalid entry
		{ nullptr, "INVALID" },
	};
	const FlagTypeProperties& FlagTypeProperties::Of(FlagType type)
	{
		const uint8_t COUNT = ARRAY_SIZE(ALL_FLAGS);
		uint8_t index = (uint8_t)type;
		// if index is invalid, set index to index of invalid entry
		if (index >= COUNT)
			index = COUNT - 1;
		return ALL_FLAGS[index];
	}
	std::optional<FlagType> ToFlagType(std::string_view codeString)
	{
		for (uint8_t i = 0; i < ARRAY_SIZE(FlagTypeProperties::ALL_FLAGS); i++)
		{
			const auto& properties = FlagTypeProperties::ALL_FLAGS[i];
			if (codeString == properties.Code) return FlagType(i);
		}
		return std::nullopt;
	}

	// Warning Type
	const WarningTypeProperties WarningTypeProperties::ALL_WARNINGS[]=
	{
		[(uint8_t)WarningType::None] = { "--", "None", "No warnings", false, false },
		[(uint8_t)WarningType::DriverDisabled] = { "FD", "DriverDisabled", "Driver disabled", false, true },
		[(uint8_t)WarningType::EncoderError] = { "FQ", "EncoderError", "Encoder error", true, false },
		[(uint8_t)WarningType::StalledAndStopped] = { "FS", "StalledAndStopped", "Stalled and stopped", true, true },
		[(uint8_t)WarningType::ExcessiveTwist] = { "FT", "ExcessiveTwist", "Excessive twist", true, true },
		[(uint8_t)WarningType::StreamBoundErrors] = { "FB", "StreamBoundErrors", "Stream bound error", true, true },
		[(uint8_t)WarningType::InterpolationDeviation] = { "FP", "InterpolationDeviation", "Interpolatated path deviation", true, false },
		[(uint8_t)WarningType::LimitError] = { "FE", "LimitError", "Limit error", true, false },
		[(uint8_t)WarningType::UnexpectedLimit] = { "WL", "UnexpectedLimit", "Unexpected limit trigger", true, false },
		[(uint8_t)WarningType::VoltageOutOfRange] = { "WV", "VoltageOutOfRange", "Voltage out of range", false, false },
		[(uint8_t)WarningType::OverTemperature] = { "WT", "OverTemperature", "System temperature high", false, false },
		[(uint8_t)WarningType::StationaryDisplacement] = { "WM", "StationaryDisplacement", "Displaced when stationary", false, false },
		[(uint8_t)WarningType::NoReference] = { "WR", "NoReference", "No reference position", false, true },
		[(uint8_t)WarningType::ManualControl] = { "NC", "ManualControl", "Manual control", false, false },
		[(uint8_t)WarningType::CommandInterrupted] = { "NI", "CommandInterrupted", "Command interrupted", false, false },
		[(uint8_t)WarningType::StreamDisconinuity] = { "ND", "StreamDisconinuity", "Stream disconinuity", false, false },
		[(uint8_t)WarningType::SettingsUpdating] = { "NU", "SettingsUpdating", "Settings updating", false, false },
		[(uint8_t)WarningType::JoystickCalibrating] = { "NJ", "JoystickCalibrating", "Joystick calibrating", false, false },
		// invalid entry
		{ nullptr, "INVALID", "INVALID", false, false },
	};
	const WarningTypeProperties& WarningTypeProperties::Of(WarningType type)
	{
		const uint8_t COUNT = ARRAY_SIZE(ALL_WARNINGS);
		uint8_t index = (uint8_t)type;
		// if index is invalid, set index to index of invalid entry
		if (index >= COUNT)
			index = COUNT - 1;
		return ALL_WARNINGS[index];
	}
	std::optional<WarningType> ToWarningType(std::string_view codeString)
	{
		for (uint8_t i = 0; i < ARRAY_SIZE(WarningTypeProperties::ALL_WARNINGS); i++)
		{
			const auto& properties = WarningTypeProperties::ALL_WARNINGS[i];
			if (codeString == properties.Code) return WarningType(i);
		}
		return std::nullopt;
	}

	// setting types
	const SettingTypeProperties SettingTypeProperties::ALL_SETTINGS[]=
	{
		[(uint8_t)SettingType::Accel] = { "accel", Scope::Axis, true },
		[(uint8_t)SettingType::CLoopCounts] = { "cloop.counts", Scope::Axis, true },
		[(uint8_t)SettingType::CLoopDisplaceTolerance] = { "cloop.displace.tolerance", Scope::Axis, true },
		[(uint8_t)SettingType::CLoopDurationMax] = { "cloop.duration.max", Scope::Axis, true },
		[(uint8_t)SettingType::CLoopMode] = { "cloop.mode", Scope::Axis, true },
		[(uint8_t)SettingType::CLoopStallTimeout] = { "cloop.stalltimeout", Scope::Axis, true },
		[(uint8_t)SettingType::CLoopSteps] = { "cloop.steps", Scope::Axis, true },
		[(uint8_t)SettingType::CommAddress] = { "comm.address", Scope::Device, true },
		[(uint8_t)SettingType::CommAlert] = { "comm.alert", Scope::Device, true },
		[(uint8_t)SettingType::CommCheckSum] = { "comm.checksum", Scope::Device, true },
		[(uint8_t)SettingType::CommProtocol] = { "comm.protocol", Scope::Device, true },
		[(uint8_t)SettingType::CommRS232Baud] = { "comm.rs232.baud", Scope::Device, true },
		[(uint8_t)SettingType::CommRS232Protocol] = { "comm.rs232.protocol", Scope::Device, true },
		[(uint8_t)SettingType::CommRS485Baud] = { "comm.rs485.baud", Scope::Device, true },
		[(uint8_t)SettingType::CommRS485Enable] = { "comm.rs485.enable", Scope::Device, true },
		[(uint8_t)SettingType::CommRS485Protocol] = { "comm.rs485.protocol", Scope::Device, true },
		[(uint8_t)SettingType::CommUSBProtocol] = { "comm.usb.protocol", Scope::Device, true },
		[(uint8_t)SettingType::DeviceID] = { "deviceid", Scope::Device, false },
		[(uint8_t)SettingType::DriverCurrentHold] = { "driver.current.hold", Scope::Axis, true },
		[(uint8_t)SettingType::DriverCurrentMax] = { "driver.current.max", Scope::Axis, false },
		[(uint8_t)SettingType::DriverCurrentRun] = { "driver.current.run", Scope::Axis, true },
		[(uint8_t)SettingType::DriverDir] = { "driver.dir", Scope::Axis, true },
		[(uint8_t)SettingType::DriverTemperature] = { "driver.temperature", Scope::Axis, false },
		[(uint8_t)SettingType::EncoderCount] = { "encoder.count", Scope::Axis, true },
		[(uint8_t)SettingType::EncoderCountH] = { "encoder.counth", Scope::Axis, true },
		[(uint8_t)SettingType::EncoderDir] = { "encoder.dir", Scope::Axis, true },
		[(uint8_t)SettingType::EncoderError] = { "encoder.error", Scope::Axis, false },
		[(uint8_t)SettingType::EncoderFilter] = { "encoder.filter", Scope::Axis, true },
		[(uint8_t)SettingType::EncoderIndexCount] = { "encoder.index.count", Scope::Axis, true },
		[(uint8_t)SettingType::EncoderIndexPhase] = { "encoder.index.phase", Scope::Axis, true },
		[(uint8_t)SettingType::EncoderMode] = { "encoder.index.mode", Scope::Axis, true },
		[(uint8_t)SettingType::EncoderPos] = { "encoder.pos", Scope::Axis, false },
		[(uint8_t)SettingType::JoyDebug] = { "joy.debug", Scope::Axis, true },
		[(uint8_t)SettingType::KnobDir] = { "knob.dir", Scope::Axis, true },
		[(uint8_t)SettingType::KnobDistance] = { "knob.distance", Scope::Axis, true },
		[(uint8_t)SettingType::KnobEnable] = { "knob.enable", Scope::Axis, true },
		[(uint8_t)SettingType::KnobMaxSpeed] = { "knob.maxspeed", Scope::Axis, true },
		[(uint8_t)SettingType::KnobMode] = { "knob.mode", Scope::Axis, true },
		[(uint8_t)SettingType::KnobSpeedProfile] = { "knob.speedprofile", Scope::Axis, true },
		[(uint8_t)SettingType::LimitApproachMaxSpeed] = { "limit.approach.maxspeed", Scope::Axis, true },
		[(uint8_t)SettingType::LimitCycleDist] = { "limit.cycle.dist", Scope::Axis, true },
		[(uint8_t)SettingType::LimitDetectDecelOnly] = { "limit.detect.decelonly", Scope::Axis, true },
		[(uint8_t)SettingType::LimitDetectMaxSpeed] = { "limit.detect.maxspeed", Scope::Axis, true },
		[(uint8_t)SettingType::LimitHomeAction] = { "limit.home.action", Scope::Axis, true },
		[(uint8_t)SettingType::LimitHomeEdge] = { "limit.home.edge", Scope::Axis, true },
		[(uint8_t)SettingType::LimitHomePos] = { "limit.home.pos", Scope::Axis, true },
		[(uint8_t)SettingType::LimitHomePosUpdate] = { "limit.home.posupdate", Scope::Axis, true },
		[(uint8_t)SettingType::LimitHomePreset] = { "limit.home.preset", Scope::Axis, true },
		[(uint8_t)SettingType::LimitHomeState] = { "limit.home.state", Scope::Axis, false },
		[(uint8_t)SettingType::LimitHomeTriggered] = { "limit.home.triggered", Scope::Axis, false },
		[(uint8_t)SettingType::LimitHomeType] = { "limit.home.type", Scope::Axis, true },
		[(uint8_t)SettingType::LimitAwayAction] = { "limit.away.action", Scope::Axis, true },
		[(uint8_t)SettingType::LimitAwayEdge] = { "limit.away.edge", Scope::Axis, true },
		[(uint8_t)SettingType::LimitAwayPos] = { "limit.away.pos", Scope::Axis, true },
		[(uint8_t)SettingType::LimitAwayPosUpdate] = { "limit.away.posupdate", Scope::Axis, true },
		[(uint8_t)SettingType::LimitAwayPreset] = { "limit.away.preset", Scope::Axis, true },
		[(uint8_t)SettingType::LimitAwayState] = { "limit.away.state", Scope::Axis, false },
		[(uint8_t)SettingType::LimitAwayTriggered] = { "limit.away.triggered", Scope::Axis, false },
		[(uint8_t)SettingType::LimitAwayType] = { "limit.away.type", Scope::Axis, true },
		[(uint8_t)SettingType::LimitCAction] = { "limit.c.action", Scope::Axis, true },
		[(uint8_t)SettingType::LimitCEdge] = { "limit.c.edge", Scope::Axis, true },
		[(uint8_t)SettingType::LimitCPos] = { "limit.c.pos", Scope::Axis, true },
		[(uint8_t)SettingType::LimitCPosUpdate] = { "limit.c.posupdate", Scope::Axis, true },
		[(uint8_t)SettingType::LimitCPreset] = { "limit.c.preset", Scope::Axis, true },
		[(uint8_t)SettingType::LimitCState] = { "limit.c.state", Scope::Axis, false },
		[(uint8_t)SettingType::LimitCTriggered] = { "limit.c.triggered", Scope::Axis, false },
		[(uint8_t)SettingType::LimitCType] = { "limit.c.type", Scope::Axis, true },
		[(uint8_t)SettingType::LimitDAction] = { "limit.d.action", Scope::Axis, true },
		[(uint8_t)SettingType::LimitDEdge] = { "limit.d.edge", Scope::Axis, true },
		[(uint8_t)SettingType::LimitDPos] = { "limit.d.pos", Scope::Axis, true },
		[(uint8_t)SettingType::LimitDPosUpdate] = { "limit.d.posupdate", Scope::Axis, true },
		[(uint8_t)SettingType::LimitDPreset] = { "limit.d.preset", Scope::Axis, true },
		[(uint8_t)SettingType::LimitDState] = { "limit.d.state", Scope::Axis, false },
		[(uint8_t)SettingType::LimitDTriggered] = { "limit.d.triggered", Scope::Axis, false },
		[(uint8_t)SettingType::LimitDType] = { "limit.d.type", Scope::Axis, true },
		[(uint8_t)SettingType::LimitMax] = { "limit.max", Scope::Axis, true },
		[(uint8_t)SettingType::LimitMin] = { "limit.min", Scope::Axis, true },
		[(uint8_t)SettingType::LimitStartPos] = { "limit.start.pos", Scope::Axis, true },
		[(uint8_t)SettingType::LimitSwapInputs] = { "limit.swapinputs", Scope::Axis, true },
		[(uint8_t)SettingType::LockstepNumgroups] = { "lockstep.numgroups", Scope::Axis, false },
		[(uint8_t)SettingType::LockstepTolerance] = { "lockstep.tolerance", Scope::Axis, true },
		[(uint8_t)SettingType::MaxSpeed] = { "maxspeed", Scope::Axis, true },
		[(uint8_t)SettingType::MotionAccelOnly] = { "motion.accelonly", Scope::Axis, true },
		[(uint8_t)SettingType::MotionDecelOnly] = { "motion.decelonly", Scope::Axis, true },
		[(uint8_t)SettingType::PeripheralID] = { "peripheralid", Scope::Axis, true },
		[(uint8_t)SettingType::Pos] = { "pos", Scope::Axis, true },
		[(uint8_t)SettingType::Resolution] = { "resolution", Scope::Axis, true },
		[(uint8_t)SettingType::StreamNumBufs] = { "stream.numbufs", Scope::Device, false },
		[(uint8_t)SettingType::StreamNumStreams] = { "stream.numstreams", Scope::Device, false },
		[(uint8_t)SettingType::SystemAccess] = { "system.access", Scope::Device, true },
		[(uint8_t)SettingType::SystemAxisCount] = { "system.axiscount", Scope::Device, false },
		[(uint8_t)SettingType::SystemCurrent] = { "system.current", Scope::Device, false },
		[(uint8_t)SettingType::SystemLEDEnable] = { "system.led.enable", Scope::Device, true },
		[(uint8_t)SettingType::SystemSerial] = { "system.serial", Scope::Device, false },
		[(uint8_t)SettingType::SystemTemperature] = { "system.temperature", Scope::Device, false },
		[(uint8_t)SettingType::SystemVoltage] = { "system.voltage", Scope::Device, false },
		[(uint8_t)SettingType::Version] = { "version", Scope::Device, false },
		[(uint8_t)SettingType::VersionBuild] = { "version.build", Scope::Device, false },
		[(uint8_t)SettingType::VirtualNumVirtual] = { "virtual.numvirtual", Scope::Device, false },
		// invalid entry
		{ "INVALID", Scope::Device, false },
	};
	const SettingTypeProperties& SettingTypeProperties::Of(SettingType type)
	{
		const uint8_t COUNT = ARRAY_SIZE(ALL_SETTINGS);
		uint8_t index = (uint8_t)type;
		// if index is invalid, set index to index of invalid entry
		if (index >= COUNT)
			index = COUNT - 1;
		return ALL_SETTINGS[index];
	}

	// Rejection Error
	const RejectionError RejectionError::ALL_REJECTION_ERRORS[] =
	{
		[(uint8_t)RejectionErrorType::None] = { "0", "None", false, false },
		[(uint8_t)RejectionErrorType::Again] = { "AGAIN", "Send again", true, true },
		[(uint8_t)RejectionErrorType::AxisOnly] = { "AXISONLY", "Axis only", false, true },
		[(uint8_t)RejectionErrorType::BadAxis] = { "BADAXIS", "Bad axis", false, true },
		[(uint8_t)RejectionErrorType::BadCommand] = { "BADCOMMAND", "Bad command", false, true },
		[(uint8_t)RejectionErrorType::BadData] = { "BADDATA", "Bad data", false, true },
		[(uint8_t)RejectionErrorType::BadMessageID] = { "BADMESSAGEID", "Bad message ID", false, false },
		[(uint8_t)RejectionErrorType::BadSplit] = { "BADSPLIT", "Bad split", false, true },
		[(uint8_t)RejectionErrorType::BrakeClosed] = { "BRAKECLOSED", "Brake closed", false, true },
		[(uint8_t)RejectionErrorType::CyclicOnly] = { "CYCLICONLY", "Cyclic only", false, true },
		[(uint8_t)RejectionErrorType::DeviceOnly] = { "DEVICEONLY", "Device only", false, true },
		[(uint8_t)RejectionErrorType::DriverDisabled] = { "DRIVERDISABLED", "Driver disabled", false, true },
		[(uint8_t)RejectionErrorType::Full] = { "FULL", "Buffer full", false, true },
		[(uint8_t)RejectionErrorType::Inactive] = { "INACTIVE", "Inactive", false, true },
		[(uint8_t)RejectionErrorType::InvalidLimits] = { "INVALIDLIMITS", "Invalid limits", false, true },
		[(uint8_t)RejectionErrorType::LockStep] = { "LOCKSTEP", "Axis is lock stepped", false, true },
		[(uint8_t)RejectionErrorType::LongWord] = { "LONGWORD", "Long word", false, true },
		[(uint8_t)RejectionErrorType::NoAccess] = { "NOACCESS", "Command not accessible", false, true },
		[(uint8_t)RejectionErrorType::NotSin] = { "NOTSIN", "NotSin", false, true },
		[(uint8_t)RejectionErrorType::NotSupported] = { "NOTSUPPORTED", "Not supported", false, true },
		[(uint8_t)RejectionErrorType::OverDriveLimit] = { "OVERDRIVELIMIT", "Over drive limit", false, true },
		[(uint8_t)RejectionErrorType::Parked] = { "PARKED", "Device is parked", false, true },
		[(uint8_t)RejectionErrorType::ProcessOn] = { "PROCESSON", "Process on", false, true },
		[(uint8_t)RejectionErrorType::Remote] = { "REMOTE", "Remote", false, true },
		[(uint8_t)RejectionErrorType::RemoteAndEnabled] = { "REMOTEANDENABLED", "Remote and enabled", false, true },
		[(uint8_t)RejectionErrorType::StatusBusy] = { "STATUSBUSY", "Status busy", true, true },
		[(uint8_t)RejectionErrorType::SystemError] = { "SYSTEMERROR", "System error", false, true },
		[(uint8_t)RejectionErrorType::Twisted] = { "TWISTED", "Twisted", false, true },
	};

	const RejectionError& RejectionError::Of(RejectionErrorType type)
	{
		const uint8_t COUNT = ARRAY_SIZE(ALL_REJECTION_ERRORS);
		uint8_t index = (uint8_t)type;
		// if index is invalid, set index to index of invalid entry
		if (index >= COUNT)
			index = COUNT - 1;
		return ALL_REJECTION_ERRORS[index];
	}

	std::optional<RejectionErrorType> ToRejectionErrorType(std::string_view codeString)
	{
		for (uint8_t i = 0; i < ARRAY_SIZE(RejectionError::ALL_REJECTION_ERRORS); i++)
		{
			const auto& properties = RejectionError::ALL_REJECTION_ERRORS[i];
			if (codeString == properties.Code) return RejectionErrorType(i);
		}
		return std::nullopt;
	}
}
